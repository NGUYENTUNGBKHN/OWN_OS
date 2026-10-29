/*
****************************************************************************************************************
 * @file       ace_os_thread_create.c
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
    ACE_OS_THREAD *saved_thread_ptr;
    UINT            saved_threshold = ((UINT) 0);
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
    thread_ptr->ace_os_thread_stack_start       = stack_start;
    thread_ptr->ace_os_thread_stack_size        = stack_size;
    thread_ptr->ace_os_thread_time_slice        = time_slice;

    /* Calculate the end of the thread's stack area */
    temp_ptr = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(stack_start);
    temp_ptr = (ACE_OS_UCHAR_POINTER_ADD(temp_ptr, (stack_size - ((ULONG)1))));
    thread_ptr->ace_os_thread_stack_end = ACE_OS_UCHAR_TO_VOID_POINTER_CONVERT(temp_ptr);

#ifndef ACE_OS_DISABLE_PREEMPTION_THRESHOLD

    /* Preemption-threshold is enabled, setup accordingly. */
    thread_ptr->ace_os_thread_preempt_threshold = preempt_threshold;

#else

    /* Preemption-threshold is disabled, determine if preemption-threshold was required.  */
    if (priority != preempt_threshold)
    {
        /* Preemption-threshold specified. Since specific preemption-threshold is not supported,
            disable all preemption. */
        thread_ptr->ace_os_thread_preempt_threshold = ((UINT) 0);
    }
    else
    {
        /* Prremption-threshold is not specified, just setup with the priority. */
        thread_ptr->ace_os_thread_preempt_threshold = priority;

    }

#endif   

    /* Set State */
    thread_ptr->ace_os_thread_state = ACE_OS_SUSPENDED;

    /* Call the target specific stack frame building routine to build the 
        thread's initial stack and to setup the actual stack pointer in the 
        control block. */
    ace_os_thread_stack_build(thread_ptr, ace_os_thread_shell_entry);

    /* Prepare to make this thread a member of the created thread list.  */
    ACE_OS_DISABLE

    /* Load the thread id field in the thread control block. */
    thread_ptr->ace_os_thread_id = ACE_OS_THREAD_ID;

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

    /* Temporarily disable preemption. */
    ace_os_thread_preempt_disable++;

    /* Determine if an automatic start was requested. If so, call the resume
       thread function and then check for a preemption condition. */ 
    if (auto_start == ACE_OS_AUTO_START)
    {
        /* Determine if the create call is being called from initialization. */
        if (ACE_OS_THREAD_GET_SYSTEM_STATE() >= ACE_OS_INITIALIZE_IN_PROGRESS)
        {
            /* Yes, this create call was made from initialization. */

            /* Pickup the current thread execute pointer, Which corresponds to the
                highest priority thread ready to execute. Interrupt lockout is
                not required, since interrupts are assmued to be disabled during
                initialization. */
            saved_thread_ptr = ace_os_thread_execute_ptr;

            /* Determine if there is thread ready for execution. */
            if (saved_thread_ptr != ACE_OS_NULL)
            {
                /* Yes, a thread is ready for execution when initialization completes. */

                /* Save the current preemption-threshold. */
                saved_threshold = saved_thread_ptr->ace_os_thread_preempt_threshold;

                /* For initialization, temporarily set the preemption-threshold to the 
                    priority level to make sure the highest-priority thread runs once
                    initialization is complete. */
                saved_thread_ptr->ace_os_thread_preempt_threshold = saved_thread_ptr->ace_os_thread_priority;
            }

        }
        else
        {
            /* Simply set the saved thread pointer to NULL. */
            saved_thread_ptr = ACE_OS_NULL;
        }

        /* Restore interrupt */
        ACE_OS_RESTORE

        /* Call the resume thread function to make this thread ready. */
        ace_os_thread_system_resume(thread_ptr);

        /* Determine if the thread's preemption-threshold needs to be restored. */
        if (saved_thread_ptr != ACE_OS_NULL)
        {
            /* Yes, restore the previous highest-priority thread's preemption-threshold. This 
                can only happen if this routine is called from initialization. */
            saved_thread_ptr->ace_os_thread_preempt_threshold = saved_threshold;
        }
    }
    else
    {
        /* Restore interrupt */
        ACE_OS_RESTORE

        /* Perform any additional activities for tool or user purpose. */


        /* Disable interrupt */
        ACE_OS_DISABLE

        /* Re-enable preemption. */
        ace_os_thread_preempt_disable--;

        /* Restore interrupt */
        ACE_OS_RESTORE

        /* Check for preemption */
        ace_os_thread_system_preempt_check();
    }

    return ACE_OS_SUCCESS;
}





/************************************************ End of file *************************************************/

