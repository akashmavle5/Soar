package edu.umich.soar.sproom.gp;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import net.java.games.input.Component;
import net.java.games.input.Controller;
import net.java.games.input.ControllerEnvironment;
import net.java.games.input.ControllerEvent;
import net.java.games.input.ControllerListener;

/**
 * Gamepad abstraction using jinput.
 *
 * @author voigtjr@gmail.com
 */
public class GamepadJInput {
	private static final Log logger = LogFactory.getLog(GamepadJInput.class);
	private static final float DEAD_ZONE_PERCENT = 0.1f;

	private static class HandlerData {
		HandlerData(Id id, Component component) {
			this.id = id;
			this.component = component;
			this.oldValue = component.getPollData();
			this.minValue = this.oldValue;
			this.maxValue = this.oldValue;
		}
		
		Component component;
		Id id;
		float oldValue;
		float minValue;
		float maxValue;
		final List<GPComponentListener> listeners = new ArrayList<GPComponentListener>();
		
		float getOld() {
			return oldValue;
		}
		
		float setOld(float v) {
			oldValue = v;
			
			if (!component.isAnalog()) {
				return v;
			}
			
			minValue = Math.min(minValue, v);
			maxValue = Math.max(maxValue, v);
			
			if (logger.isTraceEnabled()) {
				logger.trace(String.format("v%2.2f min%2.2f max%2.2f", v, minValue, maxValue));
			}
			
			// normalize to -1..1
			// there is positively a faster way to do this.
			float range = maxValue - minValue;
			if (Float.compare(range, 0) == 0) {
				return 0;
			}
			float pct = (v - minValue) / range;
			
			if (logger.isDebugEnabled()) {
				logger.debug(String.format("%s r%2.3f p%2.3f", id.name(), range, pct));
			}

			if (Math.abs(pct - 0.5f) <= DEAD_ZONE_PERCENT) {
				return 0;
			}
			
			return pct * 2.0f - 1.0f;
		}
		
		@Override
		public String toString() {
			StringBuilder sb = new StringBuilder();
			sb.append(component);
			sb.append(" ");
			sb.append(oldValue);
			sb.append(" ");
			sb.append(listeners.size());
			return sb.toString();
		}
	}
	
	public enum Id {
		OVERRIDE(null), 
		SOAR(null), 
		GPMODE(null), 
		SLOW(null), 
		LX(Component.Identifier.Axis.X), 
		LY(Component.Identifier.Axis.Y), 
		RX(Component.Identifier.Axis.Z), 
		RY(Component.Identifier.Axis.RZ);
		
		private Component.Identifier cid;
		
		private Id(Component.Identifier cid) {
			this.cid = cid;
		}
		
		private void setCId(Component.Identifier cid) {
			if (logger.isDebugEnabled()) {
				logger.debug(String.format("%s setting id to %s", this, cid));
			}
			this.cid = cid;
		}
		
		private Component.Identifier getCId() {
			return cid;
		}
	}

	private Controller controller;
	private final List<HandlerData> components = new ArrayList<HandlerData>();
	private final ScheduledExecutorService exec = Executors.newSingleThreadScheduledExecutor();
	
	public boolean isValid() {
		return controller != null;
	}
	
	public GamepadJInput() {
		ControllerEnvironment ce = ControllerEnvironment.getDefaultEnvironment();

		ce.addControllerListener(new ControllerListener() {
			@Override
			public void controllerAdded(ControllerEvent e) {
				synchronized (components) {
					if (controller == null) {
						if (e.getController().getType() == Controller.Type.GAMEPAD) {
							controller = e.getController();
						}
					}
				}
			}
			
			@Override
			public void controllerRemoved(ControllerEvent e) {
				synchronized (components) {
					if (e.getController().equals(controller)) {
						controller = null;
						components.clear();
					}
				}
			}
		});

		for (Controller c : ce.getControllers()) {
			if (c.getType() != Controller.Type.GAMEPAD) {
				continue;
			}
			
			controller = c;
			break;
		}
		
		if (controller == null) {
			return;
		}
		
		for (Id id : Id.values()) {
			if (id.getCId() != null) {
				continue;
			}
			int ordinal = id.ordinal();
			if (ordinal < controller.getComponents().length) {
				id.setCId(controller.getComponents()[ordinal].getIdentifier());
			}
		}
		
		exec.scheduleAtFixedRate(new Runnable() {
			@Override
			public void run() {
				synchronized (components) {
					controller.poll();
	
					if (logger.isTraceEnabled()) {
						for (Component c : controller.getComponents()) {
							logger.trace(String.format("%s: %1.2f", c.getName(), c.getPollData()));
						}
					}
					
					for (HandlerData data : components) {
						if (logger.isTraceEnabled()) {
							logger.trace(data);
						}
						float pollValue = data.component.getPollData();
						if (Float.compare(pollValue, data.getOld()) != 0) {
							float value = data.setOld(pollValue);
							for (GPComponentListener listener : data.listeners) {
								listener.stateChanged(data.id, value);
							}
						}
					}
				}
			}
		}, 0, 50, TimeUnit.MILLISECONDS);
	}
	
	public boolean addComponentListener(Id id, GPComponentListener listener) {
		
		synchronized (components) {
			if (controller == null) {
				logger.debug("can't add listener: no controller");
				return false;
			}
			
			logger.debug("adding listener for " + id.getCId().getName());
			Component component = controller.getComponent(id.getCId());
			if (component == null) {
				logger.error("add failed: no such component");
				return false;
			}
			
			for (HandlerData existing : components) {
				if (existing.component.equals(component)) {
					existing.listeners.add(listener);
					logger.debug("added to existing");
					return true;
				}
			}
			
			HandlerData newHandler = new HandlerData(id, component);
			newHandler.listeners.add(listener);
			components.add(newHandler);
			logger.debug("added new");
			return true;
		}
	}
	
	public void removeComponentListener(GPComponentListener listener) {
		synchronized (components) {
			for (HandlerData existing : components) {
				Iterator<GPComponentListener> iter = existing.listeners.iterator();
				while (iter.hasNext()) {
					GPComponentListener candidate = iter.next();
					if (candidate.equals(listener)) {
						iter.remove();
					}
				}
			}
		}
	}
}