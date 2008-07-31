/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.33
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package net.sourceforge.playerstage.Jplayercore;

public class MessageReplaceRule {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected MessageReplaceRule(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(MessageReplaceRule obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      playercore_javaJNI.delete_MessageReplaceRule(swigCPtr);
    }
    swigCPtr = 0;
  }

  public MessageReplaceRule(int _host, int _robot, int _interf, int _index, int _type, int _subtype, int _replace) {
    this(playercore_javaJNI.new_MessageReplaceRule(_host, _robot, _interf, _index, _type, _subtype, _replace), true);
  }

  public boolean Match(player_msghdr_t hdr) {
    return playercore_javaJNI.MessageReplaceRule_Match(swigCPtr, this, player_msghdr_t.getCPtr(hdr), hdr);
  }

  public boolean Equivalent(int _host, int _robot, int _interf, int _index, int _type, int _subtype) {
    return playercore_javaJNI.MessageReplaceRule_Equivalent(swigCPtr, this, _host, _robot, _interf, _index, _type, _subtype);
  }

  public void setReplace(int value) {
    playercore_javaJNI.MessageReplaceRule_replace_set(swigCPtr, this, value);
  }

  public int getReplace() {
    return playercore_javaJNI.MessageReplaceRule_replace_get(swigCPtr, this);
  }

  public void setNext(MessageReplaceRule value) {
    playercore_javaJNI.MessageReplaceRule_next_set(swigCPtr, this, MessageReplaceRule.getCPtr(value), value);
  }

  public MessageReplaceRule getNext() {
    long cPtr = playercore_javaJNI.MessageReplaceRule_next_get(swigCPtr, this);
    return (cPtr == 0) ? null : new MessageReplaceRule(cPtr, false);
  }

}
