/*
****************************************************************************************************************
 * @file       ace_os_thread_system_suspend.c
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
 *                              THREAD SYSTEM SUSPEND   
 * 
 * @brief      This function suspends the specified thread and changes the thread state to the value specified.
 *              *Note:
 *                  delayed suspenion processing is handled outside of this routine.
 * @param      thread_ptr    Pointer to thread to suspend
 * @return     VOID
 * 
****************************************************************************************************************
*/
VOID ace_os_thread_system_suspend(ACE_OS_THREAD *thread_ptr)
{

    ACE_OS_INTERRUPT_SAVE_AREA
    ACE_OS_THREAD *current_ptr;
    ACE_OS_THREAD *ready_next;
    ACE_OS_THREAD *ready_previous;
    UINT        priority;
    UINT        priority_bit;
    UINT        base_priority;
    ULONG       priority_map;
    ULONG       timeout;
    ULONG       combined_flags;

    /* Pickup current thread pointer */
    ACE_OS_THREAD_GET_CURRENT(current_ptr);

    /* Lockout interrupts while the thread is being suspended. */
    ACE_OS_DISABLE
    
    /* Is the current thread suspending */
    if (thread_ptr == current_ptr)
    {
        /* Pickup the wait option */

        /* Determine if an activation is needed. */
        if (timeout != ACE_OS_NO_WAIT) 
        {
            /* Make sure the suspenion is not a wait-forever. */
            if (timeout != ACE_OS_WAIT_FOREVER)
            {
                /* Activate the thread timer with the timeout value setup in the caller. */

            }
        }
        /* Yes, reset time slice for current thread.  */

    }

    /* Decrease the preempt disabled count. */
    ace_os_thread_preempt_disable --;

    /* Check to make sure the thread suspending flag is still set. If not, it
        has already been resumed. */
    if (thread_ptr->ace_os_thread_suspending == ACE_OS_TRUE)
    {
        /* Thread state change */
        ACE_OS_THREAD_STATE_CHANGE(thread_ptr, thread_ptr->ace_os_thread_state);

        /* Actually suspend this thread. But first, clear the suspending flag. */
        thread_ptr->ace_os_thread_suspending = ACE_OS_FALSE;

        /* Pickup priority */
        priority = thread_ptr->ace_os_thread_priority;

        /* Pickup the next ready thread pointer. */
        ready_next = thread_ptr->ace_os_thread_ready_next;

        /* Determine if there are other threads at this priority that are
            ready. */
        if (ready_next != thread_ptr)
        {
            /* Yes, there are other threads at this priority ready.  */

            /* Pickup the previous ready thread pointer. */
            ready_previous = thread_ptr->ace_os_thread_ready_previous;

            /* Just remove this thread from the priority list. */
            ready_next->ace_os_thread_ready_previous = ready_previous;
            ready_previous->ace_os_thread_created_next = ready_next;

            /* Determine if this is the head of the priority list. */
            if (ace_os_thread_priority_list[priority] == thread_ptr)
            {
                ace_os_thread_priority_list[priority] = ready_next;

                /* Check for a thread preempted that had preemption threshold set. */
                if (ace_os_thread_preempt_maps[MAP_INDEX] != ((ULONG)0))
                {
                    /* Ensure that this thread's priority is clear in the preempt map. */
                    ACE_OS_MOD32_BIT_SET(priority, priority_bit);
                    ace_os_thread_preempt_maps[MAP_INDEX] = ace_os_thread_preempt_maps[MAP_INDEX] & (~(priority_bit));
                }
            }
        }
        else
        {
            /* This is the only thread at this priority ready to run.  Set the head
                   pointer to NULL.  */
            ace_os_thread_priority_list[priority] = ACE_OS_NULL;

            /* Clear this priority bit in thread ready priority bit map. */
            ACE_OS_MOD32_BIT_SET(priority, priority_bit);
            ace_os_thread_priority_maps[MAP_INDEX] = ace_os_thread_priority_maps[MAP_INDEX] & (~(priority_bit));

            /* Check for a thread preempted that had preemption threshold set. */
            if (ace_os_thread_preempt_maps[MAP_INDEX] != ((ULONG)0))
            {
                /* Ensure that this thread's priority is clear in the preempt map. */
                ACE_OS_MOD32_BIT_SET(priority, priority_bit);
                ace_os_thread_preempt_maps[MAP_INDEX] = ace_os_thread_preempt_maps[MAP_INDEX] & (~(priority_bit));
            }

            /* Setup the base priority to zero. */
            base_priority = ((UINT)0);

            /* Setup working variable for the priority map. */
            priority_map = ace_os_thread_priority_maps[MAP_INDEX];

            /* Make a quick check for no other threads ready for execution. */
            if (priority_map == ((ULONG)0))
            {
                /* Nothing else is ready.  Set highest priority and execute thread
                    accordingly.  */
                ace_os_thread_highest_priority = ((UINT)ACE_OS_MAX_PRIORITIES);
                ace_os_thread_execute_ptr = ACE_OS_NULL;

                /* Resotre interrupt */
                ACE_OS_RESTORE

                /* Determine if preemption should take place. This is only possible if the current thread pointer is
                not the same as the execute thread pointer AND the system state and preempt disable flag are clear.  */
                ACE_OS_THREAD_SYSTEM_RETURN_CHECK(combined_flags)
                if (combined_flags == ((ULONG)0))
                {
                    /* preemption is needed - return to the system! */
                    ace_os_thread_system_return();
                }

                /* Return to caller */
                return;
            }
            else
            {
                /* Other threads at different priority levels are ready to run.  */

                /* Calculate the lowest bit set in the priority map. */
                ACE_OS_LOWEST_SET_BIT_CALCULATE(priority_map, priority_bit);

                /* Setup the next highest priority variable.  */
                ace_os_thread_highest_priority = base_priority + ((UINT)priority_bit);
            }
        }

        /* Determine if the suspending thread is the thread designated to execute. */
        if (thread_ptr == ace_os_thread_execute_ptr)
        {
            /* Pickup the highest priority thread to execute. */
            ace_os_thread_execute_ptr = ace_os_thread_priority_list[ace_os_thread_highest_priority];

            if (ace_os_thread_preempt_maps[MAP_INDEX] != ((ULONG)0))
            {
                /* Yes, there was a thread preempted when it was using preemption-threshold. */

                /* Disable preemption */
                ace_os_thread_preempt_disable++;

                /* Restore interrupt. */
                ACE_OS_RESTORE

                /* Interrupts are enabled briefly here to keep the interrupt
                    lockout time deterministic. */

                /* Disable interrupt. */
                ACE_OS_DISABLE

                /* Decrement the preemption disable variable. */
                ace_os_thread_preempt_disable++;

                /* Calculate the thread with preemption threshold set that
                    was interrupted by a thread above the preemption level. */

                base_priority = ((UINT)0);

                /* Setup temporary preempted maps. */
                priority_map = ace_os_thread_preempt_maps[MAP_INDEX];

                /* Calculate the lowest bit set in the priority map. */
                ACE_OS_LOWEST_SET_BIT_CALCULATE(priority_map, priority_bit);

                /* Setup the highest priority preempted thread.  */
                priority = base_priority + ((UINT)priority_bit);

                if (ace_os_thread_highest_priority >= (ace_os_thread_priority_list[priority])->ace_os_thread_preempt_threshold)
                {
                    /* Thread not allowed to execute until earlier preempted thread finishes or lowers its
                           preemption-threshold.  */
                    ace_os_thread_execute_ptr = ace_os_thread_priority_list[priority];

                    /* Clear the correspoding bit in the preempted map, since the preemption has been restored. */
                    ACE_OS_MOD32_BIT_SET(priority, priority_bit);
                    ace_os_thread_preempt_maps[MAP_INDEX] = ace_os_thread_preempt_maps[MAP_INDEX] & (~(priority_bit));
                }
            }

            /* Restore interrupt. */
            ACE_OS_RESTORE

            /* Determine if preemption should take place. This is only possible if the current thread pointer is
                not the same as the execute thread pointer AND the system state and preempt disable flag are clear.  */
            ACE_OS_THREAD_SYSTEM_RETURN_CHECK(combined_flags)
            if (combined_flags == ((ULONG) 0))
            {
                /* preemption is needed - return to the system! */
                ace_os_thread_system_return();
            }

            /* Return to caller */
            return;
        }
    }

    /* Restore interrupt */
    ACE_OS_RESTORE

    /* Determine if a preemption condition is present. */
    if (current_ptr != ace_os_thread_execute_ptr)
    {
        /* Determine if preemption should take place. This is only possible if the current thread pointer is
                not the same as the execute thread pointer AND the system state and preempt disable flag are clear.  */
        ACE_OS_THREAD_SYSTEM_RETURN_CHECK(combined_flags)
        if (combined_flags == ((ULONG)0))
        {
            /* preemption is needed - return to the system! */
            ace_os_thread_system_return();
        }
    } 

}






/************************************************ End of file *************************************************/

