/*
****************************************************************************************************************
 * @file       ace_os_thread.c
 * @brief      
 * @date       2026/08/09
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

ACE_OS_THREAD   *ace_os_thread_priority_list[ACE_OS_MAX_PRIORITIES];

ULONG           ace_os_thread_priority_maps[ACE_OS_MAX_PRIORITIES/32];

ULONG           ace_os_thread_preempt_maps[ACE_OS_MAX_PRIORITIES/32];

UINT            ace_os_thread_highest_priority;

volatile UINT    ace_os_thread_preempt_disable;

/* Define the current thread pointer. This variable points to the currently
    executing thread. If this variable is NULL, no thread is executing. */
ACE_OS_THREAD   *ace_os_thread_current_ptr;

ACE_OS_THREAD   *ace_os_thread_execute_ptr;

/***************************************************************************************************************
**                                         INTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                             FUNCTION DEFINITIONS
***************************************************************************************************************/

/*
****************************************************************************************************************
                                            CREATE A THREAD

 * @brief      
 * @param      thread_ptr    
 * @param      name_ptr    
 * @param      entry_function    
 * @param      entry_input    
 * @param      stack_start    
 * @param      stack_size    
 * @param      priority    
 * @param      time_slice    
 * @return     UINT
****************************************************************************************************************
*/
UINT ace_os_thread_create(ACE_OS_THREAD *thread_ptr, 
                          CHAR *name_ptr,
                          VOID (*entry_function)(ULONG id),
                          ULONG entry_input,
                          VOID *stack_start,
                          ULONG stack_size,
                          UINT priority,
                          UINT preempt_threshold,
                          ULONG time_slice,
                          UINT auto_start
                        )
{
    ACE_OS_THREAD *next_thread;
    ACE_OS_THREAD *prev_thread;

    UCHAR *temp_ptr;

    /* ace_os_interrupt_save_area */
    ACE_OS_INTERRUPT_SAVE_AREA

    /* Initilaize stack frame */
    ACE_OS_MEMSET(stack_start, ((UCHAR)ACE_OS_STACK_FILL), stack_size);

    /* Initialize thread control block */
    ACE_OS_MEMSET(thread_ptr, 0, sizeof(ACE_OS_THREAD));

    /* Place the supplied parameters into the thread's control block. */
    thread_ptr->ace_os_thread_name              = name_ptr;
    thread_ptr->ace_os_thread_entry             = entry_function;
    thread_ptr->ace_os_thread_entry_params      = entry_input;
    thread_ptr->ace_os_thread_priority          = priority;
    thread_ptr->ace_os_thread_preempt_threshold = preempt_threshold;
    thread_ptr->ace_os_thread_stack_start       = stack_start;
    thread_ptr->ace_os_thread_stack_size        = stack_size;
    thread_ptr->ace_os_thread_time_slice        = time_slice;

    /* Calculate the end of the thread's stack area */
    temp_ptr = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(stack_start);
    temp_ptr = (ACE_OS_UCHAR_POINTER_ADD(temp_ptr, (stack_size - ((ULONG)1))));
    thread_ptr->ace_os_thread_stack_end = ACE_OS_UCHAR_TO_VOID_POINTER_CONVERT(temp_ptr);

    /* Set State */
    thread_ptr->ace_os_thread_state = ACE_OS_SUSPENDED;

    ace_os_thread_stack_build(thread_ptr, ace_os_thread_shell_entry);

    /* Prepare to make this thread a member of the created thread list.  */
    ACE_OS_DISABLE

    // thread_ptr->ace_os_thread_id = ACE_

    /* Place the thread on the list of created threads */
    if (ace_os_thread_created_count == ACE_OS_EMPTY)
    {   
        ace_os_thread_created_ptr                   = thread_ptr;
        thread_ptr->ace_os_thread_created_next      = thread_ptr;
        thread_ptr->ace_os_thread_created_previous  = thread_ptr;
    }
    else
    {
        /* This list is not Null, add to the end of the list. */
        next_thread                 = ace_os_thread_created_ptr;
        prev_thread                 = next_thread->ace_os_thread_created_previous;

        /* Place the new thread in the list. */
        next_thread->ace_os_thread_created_previous = thread_ptr;
        prev_thread->ace_os_thread_created_next     = thread_ptr;

        /* Setup this thread's created links. */
        thread_ptr->ace_os_thread_created_previous  = prev_thread;
        thread_ptr->ace_os_thread_created_next      = next_thread;
    }

    /* Increment the thread created count.  */
    ace_os_thread_created_count++;

    /* Determine if an automatic start was requested. If so, call the resume
       thread function and then check for a preemption condition. */ 
    if (auto_start == ACE_OS_AUTO_START)
    {
        if (ACE_OS_THREAD_GET_SYSTEM_STATE() >= ACE_OS_INITIALIZE_IN_PROGRESS)
        {

        }

        /* Restore interrupt */
        ACE_OS_RESTORE

        /* Call the resume thread function to make this thread ready. */
        ace_os_thread_system_resume(thread_ptr);
    }
    else
    {
        /* Restore interrupt */
        ACE_OS_RESTORE

        /* Perform any additional activities for tool or user purpose. */


        /* Disable interrupt */
        ACE_OS_DISABLE

        /* Re-enable preemption. */

        /* Restore interrupt */
        ACE_OS_RESTORE

        /* Check for preemption */

    }

    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_delete(void)
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_entry_exit_notify(void)
{
    return ACE_OS_SUCCESS;
}

ACE_OS_THREAD  *ace_os_thread_identify(void)
{
    ACE_OS_THREAD *res = NULL;


    return res;
}

UINT ace_os_thread_info_get(void)
{
    return ACE_OS_SUCCESS;
}

VOID ace_os_thread_initialize(void)
{
    /* Note: the system stack pointer and the system state variables are
        initialized by the low and high-level initialization functions,
        respectively. */
    
    /* Set current thread pointer to NULL */
    ACE_OS_THREAD_SET_CURRENT(ACE_OS_NULL);

    /* Set execute thread pointer to NULL */
    ace_os_thread_execute_ptr = ACE_OS_NULL;

    /* Initialie the priority information */
    ACE_OS_MEMSET(&ace_os_thread_priority_maps[0], 0, (sizeof(ace_os_thread_priority_maps)));

    /* Setup the highest priority variable to the max, indicating no thread is currently
        ready. */
    ace_os_thread_highest_priority = ((UINT) ACE_OS_MAX_PRIORITIES);

    /* Initialize the array of priority head pointers. */
    ACE_OS_MEMSET(&ace_os_thread_priority_list[0], 0, (sizeof(ace_os_thread_priority_list)));

    /* Initialize the head pointer of the created threads list and the
        number of threads created. */
    ace_os_thread_created_ptr = ACE_OS_NULL;
    ace_os_thread_created_count = ACE_OS_EMPTY;

}

UINT ace_os_thread_performance_info_get(void)
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_performance_system_info_get(void)
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_preemption_change(void)
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_priority_change(void)
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_relinquish(void)
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_reset(void)
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_resume(void)
{
    return ACE_OS_SUCCESS;
}

VOID ace_os_thread_shell_entry(VOID)
{
    ACE_OS_THREAD   *thread_ptr;

    ACE_OS_INTERRUPT_SAVE_AREA
    /* Pickup thread pointer. */
    ACE_OS_THREAD_GET_CURRENT(thread_ptr);


    /* Call current thread's entry function */
    (thread_ptr->ace_os_thread_entry)(thread_ptr->ace_os_thread_entry_params);

    /* Suspend thread with a "completed" state. */

    /* Lockout interrupt while the thread state is setup. */
    ACE_OS_DISABLE

    /* Set the status to suspending, in order to indicate the suspension
        is in progress. */
    thread_ptr->ace_os_thread_state = ACE_OS_COMPLETED;

    /* Thread state change */
    // ACE_OS_THREAD_STATE

    /* Restore interrupt. */
    ACE_OS_RESTORE

    /* Call actual thread suspension routine. */
    ace_os_thread_system_suspend(thread_ptr);
}

UINT ace_os_thread_sleep(ULONG timer_ticks)
{
    ACE_OS_THREAD *thread_ptr;
    UINT            status = ACE_OS_SUCCESS;

    ACE_OS_INTERRUPT_SAVE_AREA

    /* Lockout interrupts while the thread is being resumed. */
    ACE_OS_DISABLE

    /* Pickup current thread. */
    ACE_OS_THREAD_GET_CURRENT(thread_ptr);

    /* Determine if this is a legal request */

    /* Is there a current thread? */
    if (thread_ptr == ACE_OS_NULL)
    {
        /* Restore interrupt. */
        ACE_OS_RESTORE

        /* Illegal caller of this service. */
        status = ACE_OS_CALLER_ERROR;
    }
    /* Is the caller an ISR or Initialization */
    // else if ()
    // {

    // }

    /* Is the caller the system time thread? */
    
    /* Determine if the requested number of ticks is zero */
    else if (timer_ticks == ((UINT) 0))
    {
        /* Restore interrupt. */
        ACE_OS_RESTORE

        /* Illegal caller of this service. */
        status = ACE_OS_SUCCESS;
    }
    else
    {
        /* Set state to suspended. */
        thread_ptr->ace_os_thread_state = ACE_OS_SLEEP;

        /* Call actual thread suspension routine */
        ace_os_thread_system_suspend(thread_ptr);

        /* Return status to the caller */
        
    }

    return status;
}

VOID ace_os_thread_stack_analyze(void)
{

}

VOID ace_os_thread_stack_error_handler(void)
{

}

UINT ace_os_thread_stack_error_notify(void)
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_suspend(void)
{
    return ACE_OS_SUCCESS;
}

VOID ace_os_thread_system_preempt_check(void)
{

}

VOID ace_os_thread_system_resume(ACE_OS_THREAD *thread_ptr)
{
    UINT    priority;
    UINT    priority_bit;
    ACE_OS_THREAD   *head_ptr;
    ACE_OS_THREAD   *tail_ptr;
    ACE_OS_THREAD   *execute_ptr;
    ACE_OS_THREAD   *current_thread;

    ACE_OS_INTERRUPT_SAVE_AREA

    /* Lockout interrupt while the thread is being resumed. */
    ACE_OS_DISABLE

    /* Decrease the preempt disabled count. */

    /* Determine if the thread is in the process of suspending. If so, the thread
        control block is already on the linked list so nothing needs to be done. */
    

    /* Thread is not in the process of suspending. Now check to make sure the thread
        has not already been resumed. */
    if (thread_ptr->ace_os_thread_state != ACE_OS_READY)
    {
        /* No, now check to see if the delayed suspension flag is set. */
        
        /* Resume the thread! */

        /* Make this thread ready. */

        /* Change state to ready. */
        thread_ptr->ace_os_thread_state = ACE_OS_READY;

        /* Pickup priority of thread. */
        priority = thread_ptr->ace_os_thread_priority;

        /* Thread state change. */

        /* Determine if there are other threads at this priority that are
            ready. */
        head_ptr = ace_os_thread_priority_list[priority];
        if (head_ptr == ACE_OS_NULL)
        {
            /* First thread at thí priority ready. Add to the front of the list */
            ace_os_thread_priority_list[priority]       = thread_ptr;
            thread_ptr->ace_os_thread_ready_next        = thread_ptr;
            thread_ptr->ace_os_thread_ready_previous    = thread_ptr;

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
                    }

                    return;
                }
            }

        }
        else
        {
            /* No, there are other threads at this priority already ready. */

            /* Just add this thread to the priority list. */
            tail_ptr                                    = head_ptr->ace_os_thread_ready_previous;
            tail_ptr->ace_os_thread_ready_next          = thread_ptr;
            head_ptr->ace_os_thread_ready_previous      = thread_ptr;
            thread_ptr->ace_os_thread_ready_next        = head_ptr;
            thread_ptr->ace_os_thread_ready_previous    = tail_ptr;
        }
    }


    /* Pickup thread pointer. */
    ACE_OS_THREAD_GET_CURRENT(current_thread);

    /* Restore interrupt. */
    ACE_OS_RESTORE

    /* Determine if a preemption condition is preset. */
    if (current_thread != ace_os_thread_execute_ptr)
    {

    }

}

VOID ace_os_thread_system_suspend(ACE_OS_THREAD *thread_ptr)
{
    ACE_OS_THREAD *current_ptr;
    ACE_OS_THREAD *ready_next;
    ACE_OS_THREAD *ready_previous;
    UINT        priority;
    UINT        priority_bit;
    UINT        base_priority;
    ULONG       priority_map;

    ACE_OS_INTERRUPT_SAVE_AREA

    /* Pickup current thread. */
    ACE_OS_THREAD_GET_CURRENT(current_ptr);

    /* Lockout interrupts while the thread is being suspended. */
    ACE_OS_DISABLE

    /* Check to make sure the thread suspending flag is still set.  If not, it
       has already been resumed.  */
    // if (thread_ptr -> ace_os_thread_suspending == ACE_OS_TRUE)
    // {

    /* Actually suspend this thread. But first, clear the suspending flag. */

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
            if (ace_os_thread_preempt_maps[MAP_INDEX] != ((ULONG) 0))
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
        ace_os_thread_priority_maps[MAP_INDEX] = ace_os_thread_priority_maps[MAP_INDEX] &(~(priority_bit));

        /* Check for a thread preempted that had preemption threshold set. */
        if (ace_os_thread_preempt_maps[MAP_INDEX] != ((ULONG)0))
        {
            /* Ensure that this thread's priority is clear in the preempt map. */
            ACE_OS_MOD32_BIT_SET(priority, priority_bit);
            ace_os_thread_preempt_maps[MAP_INDEX] = ace_os_thread_preempt_maps[MAP_INDEX] & (~(priority_bit));
        }

        /* Setup the base priority to zero. */
        base_priority = ((UINT) 0);

        /* Setup working variable for the priority map. */
        priority_map = ace_os_thread_priority_maps[MAP_INDEX];

        /* Make a quick check for no other threads ready for execution. */
        if (priority_map == ((ULONG)0)) 
        {
            /* Nothing else is ready.  Set highest priority and execute thread
                accordingly.  */
            ace_os_thread_highest_priority = ((UINT) ACE_OS_MAX_PRIORITIES);
            ace_os_thread_execute_ptr = ACE_OS_NULL;

            /* Resotre interrupt */
            ACE_OS_RESTORE

            return;
        }
        else
        {
            /* Other threads at different priority levels are ready to run.  */

            /* Calculate the lowest bit set in the priority map. */
            ACE_OS_LOWEST_SET_BIT_CALCULATE(priority_map, priority_bit);

            /* Setup the next highest priority variable.  */
            ace_os_thread_highest_priority = base_priority + ((UINT) priority_bit);
        }
    }

    /* Determine if the suspending thread is the thread designated to execute. */
    if (thread_ptr == ace_os_thread_execute_ptr)
    {
        /* Pickup the highest priority thread to execute. */
        ace_os_thread_execute_ptr = ace_os_thread_priority_list[ace_os_thread_highest_priority];

        if (ace_os_thread_preempt_maps[MAP_INDEX] != ((ULONG) 0))
        {
            /* Yes, there was a thread preempted when it was using preemption-threshold. */

            /* Disable preemption */
            
            /* Restore interrupt. */
            ACE_OS_RESTORE

            /* Disable interrupt. */
            ACE_OS_DISABLE

            /* Calculate the thread with preemption threshold set that
                was interrupted by a thread above the preemption level. */
            
            base_priority = ((UINT) 0);


            /* Setup temporary preempted maps. */
            priority_map = ace_os_thread_preempt_maps[MAP_INDEX];

            /* Calculate the lowest bit set in the priority map. */
            ACE_OS_LOWEST_SET_BIT_CALCULATE(priority_map, priority_bit);

            /* Setup the highest priority preempted thread.  */
            priority = base_priority + ((UINT) priority_bit);

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

        /* Return to caller */
        return;
    }
    //}


    /* Restore interrupts.  */
    ACE_OS_RESTORE

    /* Determine if a preemption condition is present.  */
    if (current_ptr != ace_os_thread_execute_ptr)
    {
       
    }

    /* Return to caller.  */
    return;
}

UINT ace_os_thread_terminate(void)
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_time_slice_change(void)
{
    return ACE_OS_SUCCESS;
}

VOID ace_os_thread_time_slice(void)
{

}

VOID ace_os_thread_timeout(void)
{

}

UINT ace_os_thread_wait_abort(void)
{
    return ACE_OS_SUCCESS;
}



/***************************************************************************************************************
**                                                End of file
***************************************************************************************************************/
