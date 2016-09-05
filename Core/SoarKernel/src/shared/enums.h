/*
 * enums.h
 *
 *  Created on: Jul 17, 2013
 *      Author: mazzin
 */

#ifndef ENUMS_H_
#define ENUMS_H_

#include "constants.h"

/* ------------------------- debug trace channels -----------------------------
 *
 * NOTE: IF YOU ADD A NEW TRACE OR DEBUG MODE, MAKE SURE TO INITIALIZE PREFIX
 *       INFO AND INITIAL VALUE IN initialize_debug_trace() in debug.cpp
 *
 * ------------------------------------------------------------------------- */
enum TraceMode
{
    No_Mode = 0,
    DT_DEBUG = 1,
    DT_ID_LEAKING = 2,
    DT_LHS_VARIABLIZATION = 3,
    DT_ADD_ADDITIONALS = 4,
    DT_RHS_VARIABLIZATION = 5,
    DT_VARIABLIZATION_MANAGER = 6,
    DT_PRINT_INSTANTIATIONS = 7,
    DT_DEALLOCATES = 8,
    DT_DEALLOCATE_SYMBOLS = 9,
    DT_REFCOUNT_ADDS = 10,
    DT_REFCOUNT_REMS = 11,
    DT_EPMEM_CMD = 12,
    DT_PARSER = 13,
    DT_MILESTONES = 14,
    DT_REORDERER = 15,
    DT_BACKTRACE = 16,
    DT_GDS = 17,
    DT_RL_VARIABLIZATION = 18,
    DT_NCC_VARIABLIZATION = 19,
    DT_IDENTITY_PROP = 20,
    DT_SOAR_INSTANCE = 21,
    DT_CLI_LIBRARIES = 22,
    DT_CONSTRAINTS = 23,
    DT_MERGE = 24,
    DT_UNGROUNDED_STI = 25,
    DT_UNIFICATION = 26,
    DT_VM_MAPS = 27,
    DT_BUILD_CHUNK_CONDS = 28,
    DT_RHS_VALUE = 29,
    DT_WME_CHANGES = 30,
    DT_DEALLOCATES_TESTS = 31,
    DT_LINKS = 32,
    DT_EBC_CLEANUP = 33,
    DT_UNKNOWN_LEVEL = 34,
    DT_RETE_PNODE_ADD = 35,
    DT_REPAIR = 36,
    DT_EXPLAIN = 37,
    DT_EXPLAIN_PATHS = 38,
    DT_EXPLAIN_ADD_INST = 39,
    DT_EXPLAIN_CONNECT = 40,
    DT_EXPLAIN_UPDATE = 41,
    DT_EXPLAIN_CONDS = 42,
    DT_EXPLAIN_IDENTITIES = 43,
    DT_UNIFY_SINGLETONS = 44,
    DT_EXTRA_RESULTS = 45,
    DT_PARSER_PROMOTE = 46,
    DT_SMEM_INSTANCE = 47,
    num_trace_modes
};

enum EBCLearnChoices { ebc_always, ebc_never, ebc_only, ebc_except };

enum ChunkingSettings {
    SETTING_EBC_LEARNING_ON,
    SETTING_EBC_ALWAYS,
    SETTING_EBC_NEVER,
    SETTING_EBC_ONLY,
    SETTING_EBC_EXCEPT,
    SETTING_EBC_BOTTOM_ONLY,
    SETTING_EBC_INTERRUPT,
    SETTING_EBC_INTERRUPT_FAILURE,
    SETTING_EBC_UTILITY_MODE,
    SETTING_EBC_IDENTITY_VRBLZ,
    SETTING_EBC_CONSTRAINTS,
    SETTING_EBC_RHS_VRBLZ,
    SETTING_EBC_OSK,
    SETTING_EBC_REPAIR_LHS,
    SETTING_EBC_REPAIR_RHS,
    SETTING_EBC_REPAIR_PROMOTION,
    SETTING_EBC_MERGE,
    SETTING_EBC_USER_SINGLETONS,
    SETTING_EBC_ALLOW_LOCAL_NEGATIONS,
    SETTING_EBC_ALLOW_OSK,
    SETTING_EBC_ALLOW_OPAQUE,
    SETTING_EBC_ALLOW_PROB,
    SETTING_EBC_ALLOW_CONFLATED,
    SETTING_EBC_ALLOW_TEMPORAL_CONSTRAINT,
    SETTING_EBC_ALLOW_LOCAL_PROMOTION,
    num_ebc_settings
 };

enum BTSourceType {
    BT_BaseInstantiation,
    BT_CDPS,
    BT_ExtraResults,
    BT_Normal
};

enum visualizationObjectType {
    viz_inst_record,
    viz_chunk_record,
    viz_simple_inst,
    viz_id_and_augs,
    viz_wme,
    viz_wme_terminal
};

enum go_type_enum { GO_PHASE, GO_ELABORATION, GO_DECISION,
                    GO_STATE, GO_OPERATOR, GO_SLOT, GO_OUTPUT
                  };

enum top_level_phase { INPUT_PHASE = 0,
                       PROPOSE_PHASE,
                       DECISION_PHASE,
                       APPLY_PHASE,
                       OUTPUT_PHASE,
                       PREFERENCE_PHASE,
                       WM_PHASE,
                       NUM_PHASE_TYPES
                     };

enum SoarCannedMessageType {
    ebc_error_max_chunks,
    ebc_error_max_dupes,
    ebc_error_invalid_chunk,
    ebc_error_invalid_justification,
    ebc_error_no_conditions,
    ebc_progress_repairing,
    ebc_progress_repaired,
};

enum EBCTraceType {
    ebc_actual_trace,
    ebc_match_trace,
    ebc_explanation_trace
};

enum EBCFailureType {
    ebc_success,
    ebc_failed_no_roots,
    ebc_failed_negative_relational_test_bindings,
    ebc_failed_reordering_rhs,
    ebc_failed_unconnected_conditions
};

enum EBCExplainStatus {
    explain_unrecorded,
    explain_recording,
    explain_recording_update,
    explain_recorded
};

enum MemoryPoolType
{
MP_float_constant,
MP_identifier,
MP_int_constant,
MP_str_constant,
MP_variable,
MP_instantiation,
MP_chunk_cond,
MP_preference,
MP_wme,
MP_output_link,
MP_io_wme,
MP_slot,
MP_gds,
MP_action,
MP_test,
MP_condition,
MP_not,
MP_production,
MP_rhs_symbol,
MP_saved_test,
MP_cons_cell,
MP_dl_cons,
MP_rete_node,
MP_rete_test,
MP_right_mem,
MP_token,
MP_alpha_mem,
MP_ms_change,
MP_node_varnames,
MP_rl_info,
MP_rl_et,
MP_rl_rule,
MP_wma_decay_element,
MP_wma_decay_set,
MP_wma_wme_oset,
MP_wma_slot_refs,
MP_epmem_wmes,
MP_epmem_info,
MP_smem_wmes,
MP_smem_info,
MP_epmem_literal,
MP_epmem_pedge,
MP_epmem_uedge,
MP_epmem_interval,
MP_constraints,
MP_attachments,
num_memory_pools
};

enum chunkNameFormats
{
    numberedFormat,
    ruleFormat
};

enum MessageType
{
    debug_msg,
    trace_msg,
    refcnt_msg
};

enum SymbolTypes
{
    VARIABLE_SYMBOL_TYPE = 0,
    IDENTIFIER_SYMBOL_TYPE = 1,
    STR_CONSTANT_SYMBOL_TYPE = 2,
    INT_CONSTANT_SYMBOL_TYPE = 3,
    FLOAT_CONSTANT_SYMBOL_TYPE = 4,
    UNDEFINED_SYMBOL_TYPE = 5
};

enum AddAdditionalTestsMode
{
    DONT_EXPLAIN,
    ALL_ORIGINALS,
    JUST_INEQUALITIES
};

enum WME_Field
{
    ID_ELEMENT = 0,
    ATTR_ELEMENT = 1,
    VALUE_ELEMENT = 2,
    NO_ELEMENT = 3
};

enum Print_Header_Type
{
    PrintBoth = 0,
    PrintAfter = 1,
    PrintBefore = 2
};

/* -- An implementation of an on/off boolean parameter --*/

enum boolean { off, on };

/* -- Possible modes for numeric indifference -- */

enum ni_mode
{
    NUMERIC_INDIFFERENT_MODE_AVG,
    NUMERIC_INDIFFERENT_MODE_SUM,
};

/* --- Types of tests (can't be 255 -- see rete.cpp) --- */

enum TestType
{
    UNINITIALIZED_TEST = 0,
    NOT_EQUAL_TEST = 1,          /* various relational tests */
    LESS_TEST = 2,
    GREATER_TEST = 3,
    LESS_OR_EQUAL_TEST = 4,
    GREATER_OR_EQUAL_TEST = 5,
    SAME_TYPE_TEST = 6,
    DISJUNCTION_TEST = 7,        /* item must be one of a list of constants */
    CONJUNCTIVE_TEST = 8,        /* item must pass each of a list of non-conjunctive tests */
    GOAL_ID_TEST = 9,            /* item must be a goal identifier */
    IMPASSE_ID_TEST = 10,        /* item must be an impasse identifier */
    EQUALITY_TEST = 11,
    SMEM_LINK_TEST = 12,
    SMEM_LINK_NOT_TEST = 13,
    SMEM_LINK_UNARY_TEST = 14,
    SMEM_LINK_UNARY_NOT_TEST = 15,
    NUM_TEST_TYPES
};

/* -------------------------------
      Types of Productions
------------------------------- */

enum ProductionType
{
    USER_PRODUCTION_TYPE,
    DEFAULT_PRODUCTION_TYPE,
    CHUNK_PRODUCTION_TYPE,
    JUSTIFICATION_PRODUCTION_TYPE,
    TEMPLATE_PRODUCTION_TYPE,
    NUM_PRODUCTION_TYPES
};

// Soar-RL assumes that the production types start at 0 and go to (NUM_PRODUCTION_TYPES-1) sequentially

/* WARNING: preference types must be numbered 0..(NUM_PREFERENCE_TYPES-1),
   because the slot structure contains an array using these indices. Also
   make sure to update the strings in prefmem.h.  Finally, make sure the
   helper function defined below (for e.g. preference_is_unary) use the
   correct indices.

   NOTE: Reconsider, binary and unary parallel preferences are all
   deprecated.  Their types are not removed here because it would break
   backward compatibility of rete fast loading/saving.  It's possible that
   can be fixed in rete.cpp, but for now, we're just keeping the preference
   types.  There is no code that actually uses them any more, though.*/

enum PreferenceType
{
    ACCEPTABLE_PREFERENCE_TYPE = 0,
    REQUIRE_PREFERENCE_TYPE = 1,
    REJECT_PREFERENCE_TYPE = 2,
    PROHIBIT_PREFERENCE_TYPE = 3,
    RECONSIDER_PREFERENCE_TYPE = 4,
    UNARY_INDIFFERENT_PREFERENCE_TYPE = 5,
    UNARY_PARALLEL_PREFERENCE_TYPE = 6,
    BEST_PREFERENCE_TYPE = 7,
    WORST_PREFERENCE_TYPE = 8,
    BINARY_INDIFFERENT_PREFERENCE_TYPE = 9,
    BINARY_PARALLEL_PREFERENCE_TYPE = 10,
    BETTER_PREFERENCE_TYPE = 11,
    WORSE_PREFERENCE_TYPE = 12,
    NUMERIC_INDIFFERENT_PREFERENCE_TYPE = 13,
    NUM_PREFERENCE_TYPES = 14,
};

/* --- types of conditions --- */
enum ConditionType {
    POSITIVE_CONDITION,
    NEGATIVE_CONDITION,
    CONJUNCTIVE_NEGATION_CONDITION
};

enum ActionType {
    MAKE_ACTION = 0,
    FUNCALL_ACTION = 1,
};

enum SupportType {
    UNKNOWN_SUPPORT = 0,
    O_SUPPORT = 1,
    I_SUPPORT = 2,
    UNDECLARED_SUPPORT = 0,
    DECLARED_O_SUPPORT = 1,
    DECLARED_I_SUPPORT = 2,
};

enum SOAR_CALLBACK_TYPE             // if you change this, update soar_callback_names
{
    NO_CALLBACK,                      /* Used for missing callback */
    AFTER_INIT_AGENT_CALLBACK,
    BEFORE_INIT_SOAR_CALLBACK,
    AFTER_INIT_SOAR_CALLBACK,
    AFTER_HALT_SOAR_CALLBACK,
    BEFORE_ELABORATION_CALLBACK,
    AFTER_ELABORATION_CALLBACK,
    BEFORE_DECISION_CYCLE_CALLBACK,
    AFTER_DECISION_CYCLE_CALLBACK,
    BEFORE_INPUT_PHASE_CALLBACK,
    INPUT_PHASE_CALLBACK,
    AFTER_INPUT_PHASE_CALLBACK,
    BEFORE_PREFERENCE_PHASE_CALLBACK,
    AFTER_PREFERENCE_PHASE_CALLBACK,
    BEFORE_WM_PHASE_CALLBACK,
    AFTER_WM_PHASE_CALLBACK,
    BEFORE_OUTPUT_PHASE_CALLBACK,
    OUTPUT_PHASE_CALLBACK,
    AFTER_OUTPUT_PHASE_CALLBACK,
    BEFORE_DECISION_PHASE_CALLBACK,
    AFTER_DECISION_PHASE_CALLBACK,
    BEFORE_PROPOSE_PHASE_CALLBACK,
    AFTER_PROPOSE_PHASE_CALLBACK,
    BEFORE_APPLY_PHASE_CALLBACK,
    AFTER_APPLY_PHASE_CALLBACK,
    WM_CHANGES_CALLBACK,
    CREATE_NEW_CONTEXT_CALLBACK,
    POP_CONTEXT_STACK_CALLBACK,
    CREATE_NEW_ATTRIBUTE_IMPASSE_CALLBACK,
    REMOVE_ATTRIBUTE_IMPASSE_CALLBACK,
    PRODUCTION_JUST_ADDED_CALLBACK,
    PRODUCTION_JUST_ABOUT_TO_BE_EXCISED_CALLBACK,
    AFTER_INTERRUPT_CALLBACK,
    AFTER_HALTED_CALLBACK,
    BEFORE_RUN_STARTS_CALLBACK,
    AFTER_RUN_ENDS_CALLBACK,
    BEFORE_RUNNING_CALLBACK,
    AFTER_RUNNING_CALLBACK,
    FIRING_CALLBACK,
    RETRACTION_CALLBACK,
    SYSTEM_PARAMETER_CHANGED_CALLBACK,
    MAX_MEMORY_USAGE_CALLBACK,
    XML_GENERATION_CALLBACK,
    PRINT_CALLBACK,
    LOG_CALLBACK,
    INPUT_WME_GARBAGE_COLLECTED_CALLBACK,
    NUMBER_OF_CALLBACKS               /* Not actually a callback   */
    /* type.  Used to indicate   */
    /* list size and MUST ALWAYS */
    /* BE LAST.                  */
} ;

enum epmem_variable_key
{
    var_rit_offset_1, var_rit_leftroot_1, var_rit_rightroot_1, var_rit_minstep_1,
    var_rit_offset_2, var_rit_leftroot_2, var_rit_rightroot_2, var_rit_minstep_2,
    var_next_id
};

enum smem_query_levels { qry_search, qry_full };
enum smem_install_type { wm_install, fake_install };
enum smem_storage_type { store_level, store_recursive }; // ways to store an identifier
enum smem_cue_element_type { attr_t, value_const_t, value_lti_t, smem_cue_element_type_none };
enum smem_variable_key { var_max_cycle, var_num_nodes, var_num_edges, var_act_thresh, var_act_mode };

#endif /* ENUMS_H_ */