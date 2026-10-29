/*
****************************************************************************************************************
 * @file       ace_os_thread_sleep.c
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
    else if (ACE_OS_THREAD_GET_SYSTEM_STATE() != ((ULONG) 0))
    {
        /* Restore interrupt */
        ACE_OS_RESTORE

        /* Illegal caller of this service */
        status = ACE_OS_CALLER_ERROR;
    }
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
        /* Determine if the preempt disable flag is non-zero. */
        if (ace_os_thread_preempt_disable != ((ULONG) 0))
        {
            /* Restore interrupt. */
            ACE_OS_RESTORE

            /* Suspension is not allowed if the preempt disable flag is non-zero at this point 
               return error completion */
            status = ACE_OS_CALLER_ERROR;
        }
        else
        {
            /* Set state to suspended. */
            thread_ptr->ace_os_thread_state = ACE_OS_SLEEP;

            /* Set the suspending flag. */
            thread_ptr->ace_os_thread_suspending = ACE_OS_TRUE;

            /* Initialize the status to successful. */
            thread_ptr->ace_os_thread_suspend_status = ACE_OS_SUCCESS;

            /* Setup the timeout period. */
            thread_ptr->ace_os_thread_timer.ace_os_timer_internal_remaining_ticks = timer_ticks; 

            /* Temporaily disable preemption. */
            ace_os_thread_preempt_disable ++;

            /* Restore interrupts. */
            ACE_OS_RESTORE

            /* Call actual thread suspension routine */
            ace_os_thread_system_suspend(thread_ptr);

            /* Return status to the caller */
            status = thread_ptr->ace_os_thread_suspend_status;
        }
    }

    return status;
}






/************************************************ End of file *************************************************/

