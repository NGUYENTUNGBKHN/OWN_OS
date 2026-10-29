/*
****************************************************************************************************************
 * @file       ace_os_thread_initialize.c
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






/************************************************ End of file *************************************************/

