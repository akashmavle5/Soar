/*
 * decider_settings.cpp
 *
 *  Created on: Sep 11, 2016
 *      Author: mazzin
 */

#include "decider_settings.h"

#include "agent.h"
#include "decider.h"
#include "ebc.h"
#include "output_manager.h"

//#include "sml_Names.h"
//#include "sml_AgentSML.h"
#include "sml_KernelSML.h"
#include "sml_Events.h"

decider_param_container::decider_param_container(agent* new_agent, uint64_t pDecider_settings[]): soar_module::param_container(new_agent)
{
    pDecider_settings[DECIDER_MAX_GP] = 20000;
    pDecider_settings[DECIDER_MAX_DC_TIME] = 0;
    pDecider_settings[DECIDER_MAX_ELABORATIONS] = 100;
    pDecider_settings[DECIDER_MAX_GOAL_DEPTH] = 100;
    pDecider_settings[DECIDER_MAX_MEMORY_USAGE] = 100000000;
    pDecider_settings[DECIDER_MAX_NIL_OUTPUT_CYCLES] = 15;
    pDecider_settings[DECIDER_WAIT_SNC] = 0;
    pDecider_settings[DECIDER_EXPLORATION_POLICY] = USER_SELECT_SOFTMAX;
    pDecider_settings[DECIDER_AUTO_REDUCE] = false;

    stop_phase = new soar_module::constant_param<top_level_phase>("stop-phase", APPLY_PHASE, new soar_module::f_predicate<top_level_phase>());
    stop_phase->add_mapping(APPLY_PHASE, "apply");
    stop_phase->add_mapping(DECISION_PHASE, "decide");
    stop_phase->add_mapping(INPUT_PHASE, "input");
    stop_phase->add_mapping(OUTPUT_PHASE, "output");
    stop_phase->add_mapping(PROPOSE_PHASE, "propose");
    add(stop_phase);

    max_gp = new soar_module::integer_param("max-gp", pDecider_settings[DECIDER_MAX_GP], new soar_module::gt_predicate<int64_t>(1, true), new soar_module::f_predicate<int64_t>());
    add(max_gp);
    max_dc_time = new soar_module::integer_param("max-dc-time", pDecider_settings[DECIDER_MAX_DC_TIME], new soar_module::gt_predicate<int64_t>(0, true), new soar_module::f_predicate<int64_t>());
    add(max_dc_time);
    max_elaborations = new soar_module::integer_param("max-elaborations", pDecider_settings[DECIDER_MAX_ELABORATIONS], new soar_module::gt_predicate<int64_t>(0, true), new soar_module::f_predicate<int64_t>());
    add(max_elaborations);
    max_goal_depth = new soar_module::integer_param("max-goal-depth", pDecider_settings[DECIDER_MAX_GOAL_DEPTH], new soar_module::gt_predicate<int64_t>(1, true), new soar_module::f_predicate<int64_t>());
    add(max_goal_depth);
    max_memory_usage = new soar_module::integer_param("max-memory-usage", pDecider_settings[DECIDER_MAX_MEMORY_USAGE], new soar_module::gt_predicate<int64_t>(1, true), new soar_module::f_predicate<int64_t>());
    add(max_memory_usage);
    max_nil_output_cycles = new soar_module::integer_param("max-nil-output-cycles", pDecider_settings[DECIDER_MAX_NIL_OUTPUT_CYCLES], new soar_module::gt_predicate<int64_t>(1, true), new soar_module::f_predicate<int64_t>());
    add(max_nil_output_cycles);
    wait_snc = new soar_module::boolean_param("wait-snc", pDecider_settings[DECIDER_WAIT_SNC] ? on : off, new soar_module::f_predicate<boolean>());
    add(wait_snc);
    timers_enabled = new soar_module::boolean_param("timers", new_agent->timers_enabled ? on : off, new soar_module::f_predicate<boolean>());
    add(timers_enabled);

    init_cmd = new soar_module::boolean_param("init", on, new soar_module::f_predicate<boolean>());
    add(init_cmd);
//    reset_cmd = new soar_module::boolean_param("reset", on, new soar_module::f_predicate<boolean>());
//    add(reset_cmd);
    stop_cmd = new soar_module::boolean_param("stop", on, new soar_module::f_predicate<boolean>());
    add(stop_cmd);
//    run_cmd = new soar_module::boolean_param("run", on, new soar_module::f_predicate<boolean>());
//    add(run_cmd);
    version_cmd = new soar_module::boolean_param("version", on, new soar_module::f_predicate<boolean>());
    add(version_cmd);

    help_cmd = new soar_module::boolean_param("help", on, new soar_module::f_predicate<boolean>());
    add(help_cmd);
    qhelp_cmd = new soar_module::boolean_param("?", on, new soar_module::f_predicate<boolean>());
    add(qhelp_cmd);

}

void decider_param_container::print_status(agent* thisAgent)
{
    std::string stateStackStr, enabledStr, disabledStr;
    int soarStackDepth;
    Output_Manager* outputManager = &Output_Manager::Get_OM();

    soarStackDepth = thisAgent->Decider->get_state_stack_string(stateStackStr);
    thisAgent->Decider->get_enabled_module_strings(enabledStr, disabledStr);
    uint64_t totalProductions =
        thisAgent->num_productions_of_type[DEFAULT_PRODUCTION_TYPE] +
        thisAgent->num_productions_of_type[USER_PRODUCTION_TYPE] +
        thisAgent->num_productions_of_type[CHUNK_PRODUCTION_TYPE];

    outputManager->reset_column_indents();
    outputManager->set_column_indent(0, 55);

    outputManager->printa(thisAgent,    "=======================================================\n");
    outputManager->printa_sf(thisAgent, "-                   Soar %s Summary                -\n", sml::sml_Names::kSoarVersionValue);
    outputManager->printa(thisAgent,    "=======================================================\n");
    outputManager->printa_sf(thisAgent, "%s\n", concatJustified("Enabled:", enabledStr.c_str(), 55).c_str());
    outputManager->printa_sf(thisAgent, "%s\n", concatJustified("Disabled:", disabledStr.c_str(), 55).c_str());
    outputManager->printa(thisAgent,    "-------------------------------------------------------\n");
    outputManager->printa_sf(thisAgent, "%s\n", concatJustified("Number of rules:", std::to_string(totalProductions).c_str(), 55).c_str());
    outputManager->printa_sf(thisAgent, "%s\n", concatJustified("Decisions", std::to_string(thisAgent->d_cycle_count).c_str(), 55).c_str());
    outputManager->printa_sf(thisAgent, "%s\n", concatJustified("Elaborations", std::to_string(thisAgent->e_cycle_count).c_str(), 55).c_str());
    outputManager->printa(thisAgent,    "-------------------------------------------------------\n");
    outputManager->printa_sf(thisAgent, "%s\n", concatJustified("State stack", stateStackStr.c_str(), 55).c_str());
    outputManager->printa_sf(thisAgent, "%s\n", concatJustified("Current number of states", std::to_string(soarStackDepth).c_str(), 55).c_str());
    outputManager->printa_sf(thisAgent, "%s\n", concatJustified("Next phase", thisAgent->outputManager->phase_to_string(thisAgent->current_phase), 55).c_str());

    outputManager->printa_sf(thisAgent, "\nFor a full list of sub-commands and settings:  soar ?");
}

void decider_param_container::print_settings(agent* thisAgent)
{
    std::string tempString;
    Output_Manager* outputManager = &Output_Manager::Get_OM();

    outputManager->reset_column_indents();
//    outputManager->set_column_indent(0, 40);
    outputManager->set_column_indent(1, 52);
    outputManager->printa(thisAgent, "====== Soar General Commands and Settings =====\n");
    outputManager->printa_sf(thisAgent, "soar ? %-%-%s\n", "Print this help listing");
    outputManager->printa_sf(thisAgent, "soar init%-%-%s\n", "Re-initializes current state of Soar");
//    outputManager->printa_sf(thisAgent, "soar reset%-%-%s\n", "Re-initializes Soar completely");
    outputManager->printa_sf(thisAgent, "soar stop [--self]%-%s\n", "Stop Soar execution");
//    outputManager->printa_sf(thisAgent, "soar run%-%-%s\n", "Run Soar");
    outputManager->printa_sf(thisAgent, "soar version%-%-%s\n", "Print version number of Soar");
    outputManager->printa(thisAgent, "----------------- Settings --------------------\n");
    outputManager->printa_sf(thisAgent, "%s   %-%s\n", concatJustified("max-elaborations", max_elaborations->get_string(), 47).c_str(), "Maximum elaboration in a decision cycle");
    outputManager->printa_sf(thisAgent, "%s   %-%s\n", concatJustified("max-goal-depth", max_goal_depth->get_string(), 47).c_str(), "Maximum goal stack depth");
    outputManager->printa_sf(thisAgent, "%s   %-%s\n", concatJustified("max-nil-output-cycles", max_nil_output_cycles->get_string(), 47).c_str(), "Impasse only after this many nil outputs");
    outputManager->printa_sf(thisAgent, "%s   %-%s\n", concatJustified("max-dc-time", max_dc_time->get_string(), 47).c_str(), "Maximum time per decision");
    outputManager->printa_sf(thisAgent, "%s   %-%s\n", concatJustified("max-memory-usage", max_memory_usage->get_string(), 47).c_str(), "Threshold for memory warning (see help)");
    outputManager->printa_sf(thisAgent, "%s   %-%s\n", concatJustified("max-gp", max_gp->get_string(), 47).c_str(), "Maximum rules gp can generate");
    outputManager->printa_sf(thisAgent, "%s   %-%s\n", concatJustified("stop-phase", stop_phase->get_string(), 47).c_str(), "Phase before which Soar will stop");
    outputManager->printa_sf(thisAgent, "%s   %-%s\n", concatJustified("timers", timers_enabled->get_string(), 47).c_str(), "Whether to profile where Soar spends its time");
    outputManager->printa_sf(thisAgent, "%s   %-%s\n", concatJustified("wait-snc", wait_snc->get_string(), 47).c_str(), "Wait after state-no-change");
    outputManager->printa(thisAgent, "-----------------------------------------------\n");
    outputManager->printa_sf(thisAgent, "\nTo change a setting: %-%- soar <setting> [<value>]\n");
    outputManager->printa_sf(thisAgent, "For a detailed explanation of these settings:  %-%-help soar\n");
}
