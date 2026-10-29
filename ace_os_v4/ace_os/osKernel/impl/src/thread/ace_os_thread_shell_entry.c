/*
****************************************************************************************************************
 * @file       ace_os_thread_shell_entry.c
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
    ACE_OS_THREAD_STATE_CHANGE(thread_ptr, ACE_OS_COMPLETED)

    /* Set the suspending flag */
    thread_ptr->ace_os_thread_suspending = ACE_OS_TRUE;
    
    /* Setup for no timeout period. */

    /* Temporarily disable preemption */
    ace_os_thread_preempt_disable++;

    /* Restore interrupt. */
    ACE_OS_RESTORE

    /* Call actual thread suspension routine. */
    ace_os_thread_system_suspend(thread_ptr);
}
/************************************************ End of file *************************************************/

