///*
// * variablization_manager_merge.cpp
// *
// *  Created on: Jul 25, 2013
// *      Author: mazzin
// */
//
//#include "variablization_manager.h"
//#include "agent.h"
//#include "instantiations.h"
//#include "assert.h"
//#include "test.h"
//#include "print.h"
//#include "debug.h"
//
//void Variablization_Manager::clear_merge_map()
//{
//    cond_merge_map->clear();
//}
//
//void Variablization_Manager::merge_values_in_conds(condition* pDestCond, condition* pSrcCond)
//{
//    dprint(DT_MERGE, "...merging conditions in attribute element...\n");
//    copy_non_identical_tests(thisAgent, &(pDestCond->data.tests.attr_test), pSrcCond->data.tests.attr_test);
//    dprint(DT_MERGE, "...merging conditions in value element...\n");
//    copy_non_identical_tests(thisAgent, &(pDestCond->data.tests.value_test), pSrcCond->data.tests.value_test);
//}
//
//condition* Variablization_Manager::get_previously_seen_cond(condition* pCond)
//{
//    std::map< Symbol*, std::map< Symbol*, std::map< Symbol*, condition*> > >::iterator iter_id;
//    std::map< Symbol*, std::map< Symbol*, condition*> >::iterator iter_attr;
//    std::map< Symbol*, condition*>::iterator iter_value;
//
//    dprint(DT_MERGE, "...looking for id equality test %s\n", pCond->data.tests.id_test->eq_test->data.referent->to_string());
//    iter_id = cond_merge_map->find(pCond->data.tests.id_test->eq_test->data.referent);
//    if (iter_id != cond_merge_map->end())
//    {
//        dprint(DT_MERGE, "...Found.  Looking  for attr equality test %s\n", pCond->data.tests.attr_test->eq_test->data.referent->to_string());
//        iter_attr = iter_id->second.find(pCond->data.tests.attr_test->eq_test->data.referent);
//        if (iter_attr != iter_id->second.end())
//        {
//            dprint(DT_MERGE, "...Found.  Looking  for value equality test %s\n", pCond->data.tests.value_test->eq_test->data.referent->to_string());
//
//            iter_value = iter_attr->second.find(pCond->data.tests.value_test->eq_test->data.referent);
//            if (iter_value != iter_attr->second.end())
//            {
//                dprint_condition(DT_MERGE, iter_value->second, "          ...found similar condition: ", true, false, false);
//                return iter_value->second;
//            }
//            else
//            {
//                dprint(DT_MERGE, "...no previously seen similar condition with that value element.\n");
//            }
//        }
//        else
//        {
//            dprint(DT_MERGE, "...no previously seen similar condition with that attribute element.\n");
//        }
//    }
//    else
//    {
//        dprint(DT_MERGE, "...no previously seen similar condition with that ID element.\n");
//    }
//
//    return NULL;
//}
//
//
///* -- Variablization_Manager::merge_conditions
// *
// *    Requires: Variablized condition list that does not have any conjunctive
// *              tests containing multiple equality tests
// *    Modifies: top_cond list (may delete entries and move non-equality tests
// *              to other conditions)
// *    Effects:  This function merges redundant conditions in a condition list
// *              by combining constraints of conditions that share identical
// *              equality tests for all three elements of the condition.
// *    Notes:    Since we are working with the variablized list, we do not
// *              need to worry about grounding id's.  The variablization
// *              logic should have already utilized that information when
// *              variablizing.  If we have the same equality symbol, we can
// *              assume they have the same grounding (or one that is unified.)
// * -- */
//
//inline int64_t count_conditions(condition* top_cond)
//{
//    int64_t count = 0;
//    for (condition* cond = top_cond; cond; cond = cond->next, count++) {}
//    return count;
//}
//
//void Variablization_Manager::merge_conditions(condition* top_cond)
//{
//    dprint(DT_MERGE, "======================\n");
//    dprint(DT_MERGE, "= Merging Conditions =\n");
//    dprint(DT_MERGE, "======================\n");
//    dprint_condition_list(DT_MERGE, top_cond, "          ", true, false, false);
//    int64_t current_cond = 1, cond_diff, new_num_conds, old_num_conds = count_conditions(top_cond);
//    dprint(DT_MERGE, "# of conditions = %lld\n", old_num_conds);
//    dprint(DT_MERGE, "======================\n");
//
//    condition* found_cond, *next_cond, *last_cond = NULL;
//    for (condition* cond = top_cond; cond; ++current_cond)
//    {
//        dprint(DT_MERGE, "Processing condition %lld: ", current_cond);
//        dprint_condition(DT_MERGE, cond, "", true, false, false);
//        next_cond = cond->next;
//        if (cond->type == POSITIVE_CONDITION)
//        {
//            /* -- Check if there already exists a condition with the same
//             *    equality tests for all three elements of the condition. -- */
//
//            found_cond = get_previously_seen_cond(cond);
//            if (found_cond)
//            {
//                /* -- Add tests in this condition to the already seen condition -- */
//                merge_values_in_conds(found_cond, cond);
//
//                /* -- Delete the redundant condition -- */
//                if (last_cond)
//                {
//                    /* -- Not at the head of the list -- */
//                    dprint(DT_MERGE, "...deleting non-head item.\n");
//                    last_cond->next = cond->next;
//                    deallocate_condition(thisAgent, cond);
//                    if (last_cond->next)
//                    {
//                        last_cond->next->prev = last_cond;
//                    }
//                    cond = last_cond;
//                }
//                else
//                {
//                    /* -- At the head of the list.  This probably can never
//                     *    occur since the head of the list will never be found
//                     *    as a previously seen condition.  -- */
//                    assert(false);
//                    dprint(DT_MERGE, "...deleting head of list.\n");
//                    top_cond = cond->next;
//                    deallocate_condition(thisAgent, cond);
//                    if (top_cond->next)
//                    {
//                        top_cond->next->prev = top_cond;
//                    }
//                    /* -- This will cause last_cond to be set to  NULL, indicating we're
//                     *    at the head of the list -- */
//                    cond = NULL;
//                }
//            }
//            else
//            {
//                /* -- First condition with these equality tests.  Add to merge map. -- */
//                dprint(DT_MERGE, "...did not find condition that matched.  Creating entry in merge map.\n");
//                (*cond_merge_map)[cond->data.tests.id_test->eq_test->data.referent][cond->data.tests.attr_test->eq_test->data.referent][cond->data.tests.value_test->eq_test->data.referent] = cond;
////                set_cond_for_id_attr_tests(cond);
//            }
//        }
//        else
//        {
//            // Search previous conditions for identical NC or NCC
//        }
//        last_cond = cond;
//        cond = next_cond;
//        dprint(DT_MERGE, "...done merging this constraint.\n");
//    }
//    dprint(DT_MERGE, "======================\n");
//    dprint_condition_list(DT_MERGE, top_cond, "          ", true, false, false);
//    new_num_conds = count_conditions(top_cond);
//    cond_diff = old_num_conds - new_num_conds;
//    dprint(DT_MERGE, "# of conditions = %lld\n", new_num_conds);
//    dprint(DT_MERGE, ((cond_diff > 0) ? "Conditions decreased by %lld conditions! (%lld - %lld)\n" : "No decrease in number of conditions. [%lld = (%lld - %lld)]\n"), cond_diff, old_num_conds, new_num_conds);
//
//    clear_merge_map();
//    dprint(DT_MERGE, "===========================\n");
//    dprint(DT_MERGE, "= Done Merging Conditions =\n");
//    dprint(DT_MERGE, "===========================\n");
//}