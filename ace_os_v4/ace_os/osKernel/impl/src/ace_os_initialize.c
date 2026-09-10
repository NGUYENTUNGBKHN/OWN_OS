/*
****************************************************************************************************************
 * @file       ace_os_initialize.c
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
#include "ace_os_initialize.h"
#include "ace_os_thread.h"
/***************************************************************************************************************
**                                         EXTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                        EXTERNAL VARIABLE DECLARATIONS
***************************************************************************************************************/
ACE_OS_THREAD * ace_os_thread_created_ptr;


ULONG ace_os_thread_created_count;

/***************************************************************************************************************
**                                          INTERNAL MACRO DEFINITIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                         COMMON VARIABLE DEFINITIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                        INTERNAL VARIABLE DEFINITIONS
***************************************************************************************************************/

/* Define the unused memory pointer. The value of the first available 
    memory address is placed in this variable in the low-level
    initialization function. The content of this variable is passed
    to the application's system definition function. */
VOID    *ace_os_initialize_unused_memory;

volatile ULONG ace_os_thread_system_state;

/***************************************************************************************************************
**                                         INTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                             FUNCTION DEFINITIONS
***************************************************************************************************************/

VOID ace_os_initialize_high_level()
{
    /* TRACE */


    /* Call the thread control initialization function. */
    ace_os_thread_initialize();

    /* Call the timer control initialization function. */
    ace_os_timer_initialize();
}

VOID ace_os_initialize_kernel_enter(VOID)
{
    /* Determine if the compiler has pre-initialized ThreadX. */
    if (ace_os_thread_system_state != ACE_OS_INITIALIZE_ALMOST_DONE)
    {
        ace_os_thread_system_state = ACE_OS_INITIALIZE_IN_PROGRESS;

        /* Invoke the low-level initialization to handle all processor specific
            initialization issues. */
        // ace_os_initialize_low_level();

        /* Invoke the high-level initialization to exercise all of the ThreadX
            components and the application's initialization function. */
        ace_os_initialize_high_level();
    }

    ace_os_thread_system_state = ACE_OS_INITIALIZE_IN_PROGRESS;

    /* Call the application provided initialization function. Pass the
        first available memory address to it. */
    ace_os_application_define(ace_os_initialize_unused_memory);

    /* Set the system state in preparation for entering the thread
        scheduler. */
    ace_os_thread_system_state = ACE_OS_INITIALIZE_IS_FINISHED;


    /* Enter the scheduling loop to start executing threads! */
    ace_os_thread_schedule();

}

VOID ace_os_initialize_kernel_setup()
{

}

/***************************************************************************************************************
**                                                End of file
***************************************************************************************************************/

