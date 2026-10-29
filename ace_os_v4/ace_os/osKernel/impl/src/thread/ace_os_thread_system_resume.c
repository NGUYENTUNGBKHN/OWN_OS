/*
****************************************************************************************************************
 * @file       ace_os_thread_system_resume.c
 * @brief      
 * @date       2026/10/29
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
/***************************************************************************************************************
**                                                   INCLUDES
***************************************************************************************************************/
#include "ace_os_api.h"
#include "ace_os_thread.h"
#include "ace_os_initialize.h"
/***************************************************************************************************************
**                                         EXTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                        EXTERNAL VARIABLE DECLARATIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                          INTERNAL MACRO DEFINITIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                         COMMON VARIABLE DEFINITIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                        INTERNAL VARIABLE DEFINITIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                         INTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                             FUNCTION DEFINITIONS
***************************************************************************************************************/

/*
****************************************************************************************************************
 *                              THREAD SYSTEM RESUME
 * 
 * @brief      This function places the specified thread on the list of ready 
 *              threads at the thread's specific proirity.
 * @param      thread_ptr    pointer to thread to resume
 * @return     VOID
 *  
 * @Calls
 *              ace_thread_system_return            Return to the system
 * @Called by
 *              ace_os_thread_create                Thread create function
****************************************************************************************************************
*/
VOID ace_os_thread_system_resume(ACE_OS_THREAD *thread_ptr)
{
    UINT            priority;
    UINT            priority_bit;
    ULONG           combined_flags;
    ACE_OS_THREAD   *head_ptr;
    ACE_OS_THREAD   *tail_ptr;
    ACE_OS_THREAD   *execute_ptr;
    ACE_OS_THREAD   *current_thread;

    ACE_OS_INTERRUPT_SAVE_AREA

    /* Lockout interrupt while the thread is being resumed. */
    ACE_OS_DISABLE

    /* Decrease the preempt disabled count. */
    ace_os_thread_preempt_disable --;

    /* Determine if the thread is in the process of suspending. If so, the thread
        control block is already on the linked list so nothing needs to be done. */
    if (thread_ptr->ace_os_thread_suspending == ACE_OS_FALSE)
    {
        /* Thread is not in the process of suspending. Now check to make sure the thread
        has not already been resumed. */
        if (thread_ptr->ace_os_thread_state != ACE_OS_READY)
        {
            /* No, now check to see if the delayed suspension flag is set. */
            if (thread_ptr->ace_os_thread_delayed_suspend == ACE_OS_FALSE)
            {
                /* Resume the thread! */

                /* Make this thread ready. */

                /* Change state to ready. */
                thread_ptr->ace_os_thread_state = ACE_OS_READY;

                /* Pickup priority of thread. */
                priority = thread_ptr->ace_os_thread_priority;

                /* Thread state change. */
                ACE_OS_THREAD_STATE_CHANGE(thread_ptr, ACE_OS_READY);

                /* Determine if there are other threads at this priority that are
                    ready. */
                head_ptr = ace_os_thread_priority_list[priority];
                if (head_ptr == ACE_OS_NULL)
                {
                    /* First thread at thí priority ready. Add to the front of the list */
                    ace_os_thread_priority_list[priority] = thread_ptr;
                    thread_ptr->ace_os_thread_ready_next = thread_ptr;
                    thread_ptr->ace_os_thread_ready_previous = thread_ptr;

                    /* Max priorities > 32 */

                    /* Or in the thread's priority bit. */
                    ACE_OS_MOD32_BIT_SET(priority, priority_bit);
                    ace_os_thread_priority_maps[MAP_INDEX] = ace_os_thread_priority_maps[MAP_INDEX] | priority_bit;

                    /* Determine if this newly ready thread is the highest priority. */
                    if (priority < ace_os_thread_highest_priority)
                    {
                        /* A new highest priority thread is present. */

                        /* Update the highest priority variable. */
                        ace_os_thread_highest_priority = priority;

                        /* Pickup the execute pointer. Since it is going to be refereced multiple
                            times, it is placed in a local variable. */
                        execute_ptr = ace_os_thread_execute_ptr;

                        /* Determine if no thread is currently executing. */
                        if (execute_ptr == ACE_OS_NULL)
                        {
                            /* Simply setup the execute pointer. */
                            ace_os_thread_execute_ptr = thread_ptr;
                        }
                        else
                        {
                            /* Another thread has been scheduled for execution. */

                            /* Check to see if this is a higher priority thread and determine if preemption is allowed. */
                            if (priority < execute_ptr->ace_os_thread_preempt_threshold)
                            {
                                if (execute_ptr->ace_os_thread_preempt_threshold != execute_ptr->ace_os_thread_priority)
                                {
                                    /* Remember that this thread was preempted by a thread above the thread's threshold. */
                                    ACE_OS_MOD32_BIT_SET(execute_ptr->ace_os_thread_priority, priority_bit);
                                    ace_os_thread_preempt_maps[MAP_INDEX] = ace_os_thread_preempt_maps[MAP_INDEX] | priority_bit;
                                }

                                ace_os_thread_execute_ptr = thread_ptr;

                                /* Restore interrupt. */
                                ACE_OS_RESTORE


                                ACE_OS_THREAD_SYSTEM_RETURN_CHECK(combined_flags);
                                if (combined_flags == ((ULONG) 0))
                                {
                                    /* Preemption is need - return to the system! */
                                    ace_os_thread_system_return();
                                }

                                return;
                            }
                        }
                    }
                }
                else
                {
                    /* No, there are other threads at this priority already ready. */

                    /* Just add this thread to the priority list. */
                    tail_ptr = head_ptr->ace_os_thread_ready_previous;
                    tail_ptr->ace_os_thread_ready_next = thread_ptr;
                    head_ptr->ace_os_thread_ready_previous = thread_ptr;
                    thread_ptr->ace_os_thread_ready_next = head_ptr;
                    thread_ptr->ace_os_thread_ready_previous = tail_ptr;
                }
            }
            /* Else, delayed suspend flag was set. */
            else
            {   
                /* Clear the delayed suspend flag and change the state */
                thread_ptr->ace_os_thread_delayed_suspend = ACE_OS_FALSE;
                thread_ptr->ace_os_thread_state = ACE_OS_SUSPENDED;
            }
        }
    }
    else
    {
        /* A resumption occurred in the middle of a previous thread suspension. */

        /* Make sure the type of suspenion under way is not a terminate or
            thread completion.  In either of these cases, do not void the 
            interrupted suspension processing. */
        if (thread_ptr->ace_os_thread_state != ACE_OS_COMPLETED)
        {
            /* Make sure the thread isn't terminated. */
            if (thread_ptr->ace_os_thread_state != ACE_OS_TERMINATED)
            {
                /* No, now check to see if the delayed suspension flag is set. */
                if (thread_ptr->ace_os_thread_delayed_suspend == ACE_OS_FALSE)
                {
                    /* Clear the suspending flag. */ 
                    thread_ptr->ace_os_thread_suspending = ACE_OS_FALSE;

                    /* Restore the sate to ready. */
                    thread_ptr->ace_os_thread_state = ACE_OS_READY;
                }
                else
                {
                    /* Clear the delayed suspend flag and cahnge the state. */
                    thread_ptr->ace_os_thread_delayed_suspend = ACE_OS_FALSE;
                    thread_ptr->ace_os_thread_state = ACE_OS_SUSPENDED;
                }
            }
        }
    }
    
    /* Pickup thread pointer. */
    ACE_OS_THREAD_GET_CURRENT(current_thread);

    /* Restore interrupt. */
    ACE_OS_RESTORE

    /* Determine if a preemption condition is preset. */
    if (current_thread != ace_os_thread_execute_ptr)
    {
        ACE_OS_THREAD_SYSTEM_RETURN_CHECK(combined_flags);
        if (combined_flags == ((ULONG)0))
        {
            /* Preemption is need - return to the system! */
            ace_os_thread_system_return();
        }
    }

}







/************************************************ End of file *************************************************/

