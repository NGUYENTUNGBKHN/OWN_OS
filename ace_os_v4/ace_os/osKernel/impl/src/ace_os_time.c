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

/* Define the system clock value that is continually incremented by the
   periodic timer interrupt processing.  */

volatile ULONG      ace_os_timer_system_clock;

/* Define the time-slice expiration flag.  This is used to indicate that a time-slice
   has happened.  */

UINT                ace_os_timer_expired_time_slice;

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

/* Define the timer expiration flag.  This is used to indicate that a timer
   has expired.  */

UINT            ace_os_timer_expired;

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

/* Define the current tinme slice value. If non-zero, a time-slice is active
    otherwise, the time_slice is not active. */
    
ULONG           ace_os_timer_time_slice;

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
    return ACE_OS_SUCCESS;
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
    // UINT    status;

    // /* Initializa the system clock to 0. */
    // ace_os_timer_system_clock = ((ULONG) 0);

    // /* Initialize the time-slice value to 0 make sure it is disabled */
    // ace_os_timer_time_slice = ((ULONG) 0);

    // /* Clear the expired flags */

    // /* Set the currently expired timer being processedpointer to NULL. */
    
    // /* Initialize the thread and application timer management control structures. */

    // /* First, initialize the timer list. */
    // ACE_OS_MEMSET(&ace_os_timer_list[0], 0, (sizeof(ace_os_timer_list)));

    // /* Initialize all of the list pointer. */
    // ace_os_timer_list_start = &ace_os_timer_list[0];
    // ace_os_timer_current_ptr = &ace_os_timer_list[0];

    // /* Set the timer list end pointer to one past the actual timer list.  This is done
    //    to make the timer interrupt handling in assembly language a little easier.  */
    
    // ace_os_timer_list_end = &ace_os_timer_list[ACE_OS_TIMER_ENTRIES-((ULONG) 1)];
    // ace_os_timer_list_end = ACE_OS_TIMER_POINTER_ADD(ace_os_timer_list_end, ((ULONG) 1));

    // /* Setup the variables associated with the system timer thread's stack and
    //    priority.  */
    // ace_os_timer_stack_start =  (VOID *) &ace_os_timer_thread_stack_area[0];
    // ace_os_timer_stack_size =   ((ULONG) ACE_OS_TIMER_THREAD_STACK_SIZE);
    // ace_os_timer_priority =     ((UINT) ACE_OS_TIMER_THREAD_PRIORITY);

    // /* Create the system timer thread.  This thread processes all of the timer
    //    expirations and reschedules.  Its stack and priority are defined in the
    //    low-level initialization component.  */
    // do
    // {
    //     status =  _tx_thread_create(&ace_os_timer_thread,
    //                                 ACE_OS_CONST_CHAR_TO_CHAR_POINTER_CONVERT("System Timer Thread"),
    //                                 ace_os_timer_thread_entry,
    //                                 ((ULONG) ACE_OS_TIMER_ID),
    //                                 ace_os_timer_stack_start, ace_os_timer_stack_size,
    //                                 ace_os_timer_priority, ace_os_timer_priority, ACE_OS_NO_TIME_SLICE, ACE_OS_DONT_START);
    
    // } while (status != ACE_OS_SUCCESS);
    
    // /* Initialize the head pointer of the created application timer list.  */
    // ace_os_timer_created_ptr =  ACE_OS_NULL;

    // /* Set the created count to zero.  */
    // ace_os_timer_created_count =  ACE_OS_EMPTY;
}

UINT ace_os_timer_performance_info_get()
{
    return ACE_OS_SUCCESS;
}

UINT ace_os_timer_performance_system_info_get()
{
    return ACE_OS_SUCCESS;
}

VOID ace_os_timer_system_activate(ACE_OS_TIMER_INTERNAL *timer_ptr)
{
    (void*)(timer_ptr++);
}

VOID ace_os_timer_system_deactivate(ACE_OS_TIMER_INTERNAL *timer_ptr)
{
    (void*)(timer_ptr++);
}

VOID ace_os_timer_thread_entry(ULONG timer_thread_input)
{
    ACE_OS_INTERRUPT_SAVE_AREA

    ACE_OS_TIMER_INTERNAL   *expired_timers;
    ACE_OS_TIMER_INTERNAL   *current_timer;
    ACE_OS_TIMER_INTERNAL   *next_timer;
    // ACE_OS_TIMER_INTERNAL   *reactivate_timer;
    ACE_OS_TIMER_INTERNAL   *previous_timer;

    /* Make sure the timer input is correct. This also gets rid of the
        silly compiler warings. */
    if (timer_thread_input == ACE_OS_TIMER_ID)
    {


        /* Yes, valid therad entry, proceed.. */

        /* Now go into an infinite loop to process timer expirations. */
        while (ACE_OS_LOOP_FOREVER)
        {
            /* First, move the current list pointer and clear the timer
                expired value. This allows the interrupt handling portion
                to continue looking for timer expirations. */
            ACE_OS_DISABLE

            /* Save the current timer expiration list pointer */
            expired_timers = *ace_os_timer_current_ptr;

            /* Modify the head pointer in the first timer in the list, if there
                is one. */
            if (expired_timers != ACE_OS_NULL)
            {
                expired_timers->ace_os_timer_internal_list_head = &expired_timers;
            } 

            /* Set the current list pointer to NULL */
            *ace_os_timer_current_ptr = ACE_OS_NULL;

            /* Move the current pointer up one timer entry wrap if we get to 
                the end of the list. */
            ace_os_timer_current_ptr = ACE_OS_TIMER_POINTER_ADD(ace_os_timer_current_ptr, 1);
            if (ace_os_timer_current_ptr == ace_os_timer_list_end)
            {
                ace_os_timer_current_ptr = ace_os_timer_list_start;
            }

            /* Clear the expired flag. */
            ace_os_timer_expired = ACE_OS_FALSE;

            /* Restore interrupt tempporarily. */
            ACE_OS_RESTORE

            /* Disable interrupts again. */
            ACE_OS_DISABLE

            /* Next, process the expiration of the associated timers at this
                time slot. */
            while (expired_timers != ACE_OS_NULL)
            {
                /* Something is on the list. Remove it and process the expiration. */
                current_timer = expired_timers;

                /* Pickup the next timer. */
                next_timer = expired_timers->ace_os_timer_internal_active_next;

                /* Set the reactivate_timer_to NULL */
                // reactivate_timer = ACE_OS_NULL;

                /* Determine if this is the only timer. */
                if (current_timer == next_timer)
                {
                    /* Yes, this is the only timer in the list. */

                    /* Set the hread pointer to NULL. */
                    expired_timers = ACE_OS_NULL;

                }
                else
                {
                    /* No, not the only expired timer. */

                    /* Remove this timer from the expired list. */
                    previous_timer = current_timer->ace_os_timer_internal_active_previous;
                    next_timer->ace_os_timer_internal_active_previous = previous_timer;
                    previous_timer->ace_os_timer_internal_active_next = next_timer;
                }
            }
            

        }
        
    }
}

/***************************************************************************************************************
**                                                End of file
***************************************************************************************************************/

