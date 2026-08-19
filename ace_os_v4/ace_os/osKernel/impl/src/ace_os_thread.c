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
    thread_ptr->ace_os_thread_name          = name_ptr;
    thread_ptr->ace_os_thread_entry         = entry_function;
    thread_ptr->ace_os_thread_entry_params  = entry_input;
    thread_ptr->ace_os_thread_priority      = priority;
    thread_ptr->ace_os_thread_stack_start   = stack_start;
    thread_ptr->ace_os_thread_stack_size    = stack_size;

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

    if (auto_start == ACE_OS_AUTO_START)
    {
        /* Restore interrupt */
        ACE_OS_RESTORE
    }
    else
    {
        /* Restore interrupt */

        /* Perform any additional activities for tool or user purpose. */

        /* Disable interrupt */

        /* Re-enable preemption. */

        /* Restore interrupt */
        ACE_OS_RESTORE

        /* Check for preemption */

    }

    return ACE_OS_SUCCESS;
}

UINT ace_os_thread_delete(void)
{
    
}

UINT ace_os_thread_entry_exit_notify(void)
{

}

ACE_OS_THREAD  *ace_os_thread_identify(void)
{

}

UINT ace_os_thread_info_get(void)
{

}

VOID ace_os_thread_initialize(void)
{

}

UINT ace_os_thread_performance_info_get(void)
{

}

UINT ace_os_thread_performance_system_info_get(void)
{

}

UINT ace_os_thread_preemption_change(void)
{

}

UINT ace_os_thread_priority_change(void)
{

}

UINT ace_os_thread_relinquish(void)
{

}

UINT ace_os_thread_reset(void)
{

}

UINT ace_os_thread_resume(void)
{

}

VOID ace_os_thread_shell_entry(ACE_OS_THREAD *thread_ptr, VOID (*function_ptr)(VOID))
{

}

UINT ace_os_thread_sleep(void)
{

}

VOID ace_os_thread_stack_analyze(void)
{

}

VOID ace_os_thread_stack_error_handler(void)
{

}

UINT ace_os_thread_stack_error_notify(void)
{

}

UINT ace_os_thread_suspend(void)
{

}

VOID ace_os_thread_system_preempt_check(void)
{

}

VOID ace_os_thread_system_resume(void)
{

}

VOID ace_os_thread_system_suspend(void)
{

}

UINT ace_os_thread_terminate(void)
{

}

UINT ace_os_thread_time_slice_change(void)
{
    
}

VOID ace_os_thread_time_slice(void)
{

}

VOID ace_os_thread_timeout(void)
{

}

UINT ace_os_thread_wait_abort(void)
{

}



/***************************************************************************************************************
**                                                End of file
***************************************************************************************************************/
