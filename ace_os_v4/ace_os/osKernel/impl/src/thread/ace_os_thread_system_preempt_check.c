/*
****************************************************************************************************************
 * @file       ace_os_thread_system_preempt_check.c
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
 *                          SYSTEM PREEMPTION CHECK
 * 
 * @brief      This function checks for preemption that could have occurred as a result scheduling activities
 *              occuring while the preempt disable flag was set.
 * @return     VOID
 * 
****************************************************************************************************************
*/
VOID ace_os_thread_system_preempt_check(void)
{
    ULONG           combined_flags;
    ACE_OS_THREAD   *current_thread;
    ACE_OS_THREAD   *thread_ptr;

    /* Combine the system state and preempt disable flags into one for comparison. */
    ACE_OS_THREAD_SYSTEM_RETURN_CHECK(combined_flags);
    
    /* Determine if we are in a system state (ISR or Initialization) or internal preemption
        is disabled */
    if (combined_flags == ((ULONG) 0))
    {
        /* No, at thread execute level so continue checking for preemption. */

        /* Pickup thread pointer */
        ACE_OS_THREAD_GET_CURRENT(current_thread);

        /* Pickup the next execute pointer */
        thread_ptr = ace_os_thread_execute_ptr;

        /* Determine if preemption should take place. */
        if (current_thread != thread_ptr)
        {
            /* Return to the system so the higher priority thread can be scheduled. */
            ace_os_thread_system_return();
        }
    }
}






/************************************************ End of file *************************************************/
