/*
****************************************************************************************************************
 * @file       ace_os_time.c
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
#include "ace_os_time.h"
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

/* Define the thread and application timer entry list. This list provide a direct access
    method for insertion of times less than ACE_OS_TIMER_ENTRIES. */

ACE_OS_TIMER_INTERNAL *ace_os_timer_list[ACE_OS_TIMER_ENTRIES];

/* Define the boundary pointers to the list. These are setup to easily manage
    wrapping the list. */

ACE_OS_TIMER_INTERNAL   **ace_os_timer_list_start;
ACE_OS_TIMER_INTERNAL   **ace_os_timer_list_end;

/* Define the current timer pointer in the list.  This pointer is moved sequentially
   through the timer list by the timer interrupt handler.  */

ACE_OS_TIMER_INTERNAL **ace_os_timer_current_ptr;

/* Define the current tinme slice value. If non-zero, a time-slice is active
    otherwise, the time_slice is not active. */
    
ULONG           ace_os_timer_time_slice;



/* Define the created timer list head pointer.  */

ACE_OS_TIMER            *ace_os_timer_created_ptr;


/* Define the created timer count.  */

ULONG               ace_os_timer_created_count;

/* Define the timer thread's control block. */

ACE_OS_THREAD   ace_os_timer_thread;

/* Define the variable that holds the timer thread's starting stack address. */

VOID            *ace_os_timer_stack_start;

/* Define the variable that holds the timer thread's stack size. */

ULONG           ace_os_timer_stack_size;

/* Define the variable that holds the timer thread's priority. */

UINT            ace_os_timer_priority;

/* Define the system timer thread'stack. The default size is defined
    in ace_os_port.h */

ULONG           ace_os_timer_thread_stack_area[(((UINT) ACE_OS_TIMER_THREAD_STACK_SIZE)+((sizeof(ULONG))- ((UINT) 1)))/(sizeof(ULONG))];



/***************************************************************************************************************
**                                         INTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                             FUNCTION DEFINITIONS
***************************************************************************************************************/

ULONG ace_os_time_get()
{
    return ACE_OS_SUCCESS;
}

VOID ace_os_time_set()
{

}

UINT ace_os_timer_activate()
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_timer_change()
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_timer_create()
{

}

UINT ace_os_timer_deactivate()
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_timer_delete()
{
    return ACE_OS_SUCCESS;
}

VOID ace_os_timer_expriration_process()
{

}

UINT ace_os_timer_info_get()
{
    return ACE_OS_SUCCESS;
}

VOID ace_os_timer_initialize(VOID)
{
    UINT    status;

    /* Initializa the system clock to 0. */
    ace_os_timer_system_clock = ((ULONG) 0);

    /* Initialize the time-slice value to 0 make sure it is disabled */
    ace_os_timer_time_slice = ((ULONG) 0);

    /* Clear the expired flags */

    /* Set the currently expired timer being processedpointer to NULL. */
    
    /* Initialize the thread and application timer management control structures. */

    /* First, initialize the timer list. */
    ACE_OS_MEMSET(&ace_os_timer_list[0], 0, (sizeof(ace_os_timer_list)));

    /* Initialize all of the list pointer. */
    ace_os_timer_list_start = &ace_os_timer_list[0];
    ace_os_timer_current_ptr = &ace_os_timer_list[0];

    /* Set the timer list end pointer to one past the actual timer list.  This is done
       to make the timer interrupt handling in assembly language a little easier.  */
    
    ace_os_timer_list_end = &ace_os_timer_list[ACE_OS_TIMER_ENTRIES-((ULONG) 1)];
    ace_os_timer_list_end = ACE_OS_TIMER_POINTER_ADD(ace_os_timer_list_end, ((ULONG) 1));

    /* Setup the variables associated with the system timer thread's stack and
       priority.  */
    ace_os_timer_stack_start =  (VOID *) &ace_os_timer_thread_stack_area[0];
    ace_os_timer_stack_size =   ((ULONG) ACE_OS_TIMER_THREAD_STACK_SIZE);
    ace_os_timer_priority =     ((UINT) ACE_OS_TIMER_THREAD_PRIORITY);

    /* Create the system timer thread.  This thread processes all of the timer
       expirations and reschedules.  Its stack and priority are defined in the
       low-level initialization component.  */
    do
    {
        status =  _tx_thread_create(&ace_os_timer_thread,
                                    ACE_OS_CONST_CHAR_TO_CHAR_POINTER_CONVERT("System Timer Thread"),
                                    ace_os_timer_thread_entry,
                                    ((ULONG) ACE_OS_TIMER_ID),
                                    ace_os_timer_stack_start, ace_os_timer_stack_size,
                                    ace_os_timer_priority, ace_os_timer_priority, ACE_OS_NO_TIME_SLICE, ACE_OS_DONT_START);
    
    } while (status != ACE_OS_SUCCESS);
    
    /* Initialize the head pointer of the created application timer list.  */
    ace_os_timer_created_ptr =  ACE_OS_NULL;

    /* Set the created count to zero.  */
    ace_os_timer_created_count =  ACE_OS_EMPTY;
}

UINT ace_os_timer_performance_info_get()
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_timer_performance_system_info_get()
{
    return ACE_OS_SUCCESS;
}

VOID ace_os_timer_system_activate()
{

}

VOID ace_os_timer_system_deactivate()
{

}

VOID ace_os_timer_thread_entry()
{

}

/***************************************************************************************************************
**                                                End of file
***************************************************************************************************************/

