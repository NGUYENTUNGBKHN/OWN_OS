/*
****************************************************************************************************************
 * @file       ace_os_time.h
 * @brief      
 * @date       2026/08/09
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
#ifndef _ACE_OS_TIME_H_
#define _ACE_OS_TIME_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */

/* Define timer management specific data definitions. */
#define ACE_OS_TIMER_ID                 ((ULONG) 0x4154494D)
#define ACE_OS_TIMER_ENTRIES            ((ULONG) 32)


/* Define internal timer management function prototypes. */

VOID ace_os_timer_expriration_process(VOID);
VOID ace_os_timer_initialize(VOID);
VOID ace_os_timer_system_activate(ACE_OS_TIMER_INTERNAL *timer_ptr);
VOID ace_os_timer_system_deactivate(ACE_OS_TIMER_INTERNAL *timer_ptr);
VOID ace_os_timer_thread_entry(ULONG timer_thread_input);

#define TIMER_DECLARE extern

/* Define the system clock value that is continually incremented by the
    periodic timer interrupt processing. */

TIMER_DECLARE volatile ULONG    ace_os_timer_system_clock;


/* Define the current tinme slice value. If non-zero, a time-slice is active
    otherwise, the time_slice is not active. */

TIMER_DECLARE ULONG             ace_os_timer_time_slice;

/* Define the thread and application timer entry list.  This list provides a direct access
   method for insertion of times less than ACE_OS_TIMER_ENTRIES.  */

TIMER_DECLARE   ACE_OS_TIMER_INTERNAL *ace_os_timer_list[ACE_OS_TIMER_ENTRIES];

/* Define the boundary pointers to the list. These are setup to easily manage
    wrapping the list. */

TIMER_DECLARE ACE_OS_TIMER_INTERNAL   **ace_os_timer_list_start;
TIMER_DECLARE ACE_OS_TIMER_INTERNAL   **ace_os_timer_list_end;


/* Define the created timer list head pointer.  */

TIMER_DECLARE ACE_OS_TIMER            *ace_os_timer_created_ptr;


/* Define the created timer count.  */

TIMER_DECLARE ULONG               ace_os_timer_created_count;

/* Define the timer thread's control block. */

TIMER_DECLARE ACE_OS_THREAD   ace_os_timer_thread;

/* Define the variable that holds the timer thread's starting stack address. */

TIMER_DECLARE VOID            *ace_os_timer_stack_start;

/* Define the variable that holds the timer thread's stack size. */

TIMER_DECLARE ULONG           ace_os_timer_stack_size;

/* Define the variable that holds the timer thread's priority. */

TIMER_DECLARE UINT            ace_os_timer_priority;

/* Define the system timer thread'stack. The default size is defined
    in ace_os_port.h */

TIMER_DECLARE ULONG           ace_os_timer_thread_stack_area[(((UINT) ACE_OS_TIMER_THREAD_STACK_SIZE)+((sizeof(ULONG))- ((UINT) 1)))/(sizeof(ULONG))];


#ifdef __cplusplus
}
#endif
#endif
