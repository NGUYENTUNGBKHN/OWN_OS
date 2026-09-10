/*
****************************************************************************************************************
 * @file       ace_os_thread.h
 * @brief      
 * @date       2026/08/09
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
#ifndef _ACE_OS_THREAD_H_
#define _ACE_OS_THREAD_H_
#ifdef __cplusplus
extern "C"
{
#endif


/* Define the MOD32 bit set macro that is used to set/clear a priority bit within a specific
   priority group. */

#define MAP_INDEX                      0
#ifndef ACE_OS_MOD32_BIT_SET
#define ACE_OS_MOD32_BIT_SET(a,b)      (b) = (((ULONG) 1) << ((a)));
#endif

/* Define the macro to get the current thread pointer. This is particularly useful in SMP
   versions of ThreadX to add additional processing.  The default implementation is to simply
   access the global current thread pointer directly.  */

#ifndef ACE_OS_THREAD_GET_CURRENT
#define ACE_OS_THREAD_GET_CURRENT(a)            (a) =  ace_os_thread_current_ptr;
#endif

/* Define the macro to set the current thread pointer. This is particularly useful in SMP
   versions of ThreadX to add additional processing.  The default implementation is to simply
   access the global current thread pointer directly.  */

#ifndef ACE_OS_THREAD_SET_CURRENT
#define ACE_OS_THREAD_SET_CURRENT(a)            ace_os_thread_current_ptr = (a);
#endif 

/* Define the lowest bit set macro. Note, that this may be overridden
   by a port specific definition if there is supporting assembly language
   instructions in the architecture.  */

#ifndef ACE_OS_LOWEST_SET_BIT_CALCULATE
#define ACE_OS_LOWEST_SET_BIT_CALCULATE(m, b)       \
    (b) =  ((ULONG) 0);                         \
    (m) =  (m) & ((~(m)) + ((ULONG) 1));        \
    if ((m) < ((ULONG) 0x10))                   \
    {                                           \
        if ((m) >= ((ULONG) 4))                 \
        {                                       \
            (m) = (m) >> ((ULONG) 2);           \
            (b) = (b) + ((ULONG) 2);            \
        }                                       \
        (b) = (b) + ((m) >> ((ULONG) 1));       \
    }                                           \
    else if ((m) < ((ULONG) 0x100))             \
    {                                           \
        (m) = (m) >> ((ULONG) 4);               \
        (b) = (b) + ((ULONG) 4);                \
        if ((m) >= ((ULONG) 4))                 \
        {                                       \
            (m) = (m) >> ((ULONG) 2);           \
            (b) = (b) + ((ULONG) 2);            \
        }                                       \
        (b) = (b) + ((m) >> ((ULONG) 1));       \
    }                                           \
    else if ((m) < ((ULONG) 0x10000))           \
    {                                           \
        (m) = (m) >> ((ULONG) 8);               \
        (b) = (b) + ((ULONG) 8);                \
        if ((m) >= ((ULONG) 0x10))              \
        {                                       \
            (m) = (m) >> ((ULONG) 4);           \
            (b) = (b) + ((ULONG) 4);            \
        }                                       \
        if ((m) >= ((ULONG) 4))                 \
        {                                       \
            (m) = (m) >> ((ULONG) 2);           \
            (b) = (b) + ((ULONG) 2);            \
        }                                       \
        (b) = (b) + ((m) >> ((ULONG) 1));       \
    }                                           \
    else                                        \
    {                                           \
        (m) = (m) >> ((ULONG) 16);              \
        (b) = (b) + ((ULONG) 16);               \
        if ((m) >= ((ULONG) 0x100))             \
        {                                       \
            (m) = (m) >> ((ULONG) 8);           \
            (b) = (b) + ((ULONG) 8);            \
        }                                       \
        if ((m) >= ((ULONG) 16))                \
        {                                       \
            (m) = (m) >> ((ULONG) 4);           \
            (b) = (b) + ((ULONG) 4);            \
        }                                       \
        if ((m) >= ((ULONG) 4))                 \
        {                                       \
            (m) = (m) >> ((ULONG) 2);           \
            (b) = (b) + ((ULONG) 2);            \
        }                                       \
        (b) = (b) + ((m) >> ((ULONG) 1));       \
    }
#endif



/* Define internal thread control function prototypes */
VOID ace_os_thread_schedule(VOID);
VOID ace_os_thread_stack_build(ACE_OS_THREAD *thread_ptr, VOID (*function_ptr)(VOID));
VOID ace_os_thread_system_preempt_check(VOID);
VOID ace_os_thread_system_resume(ACE_OS_THREAD *thread_ptr);
VOID ace_os_thread_system_suspend(ACE_OS_THREAD *thread_ptr);
VOID ace_os_thread_shell_entry(VOID);
VOID ace_os_thread_stack_analyze(VOID);
VOID ace_os_thread_stack_error_handler(VOID);
VOID ace_os_thread_time_slice(VOID);
VOID ace_os_thread_timeout(VOID);
VOID ace_os_thread_system_return(VOID);

#define THREAD_DECLARE  extern

/* Define the pointer that contains the system stack pointer. This is
   utilized when control returns form a thread to the system to reset the
   current stack. This is setup in the low-level initialization function. */

THREAD_DECLARE VOID*    ace_os_thread_system_stack_ptr;

/* Define the current thread pointe. This variable points to the currently
   executing thread. If this variable is NULL, no-thread is executing. */

THREAD_DECLARE ACE_OS_THREAD*    ace_os_thread_current_ptr;


THREAD_DECLARE ACE_OS_THREAD*    ace_os_thread_execute_ptr;

/* Define the head pointer of the created thread list.  */

THREAD_DECLARE ACE_OS_THREAD * ace_os_thread_created_ptr;

/* Define the variable that holds the number of created threads. */

THREAD_DECLARE ULONG ace_os_thread_created_count;


/* Define the current state variable.  When this value is 0, a thread
   is executing or the system is idle.  Other values indicate that
   interrupt or initialization processing is active.  This variable is
   initialized to ACE_OS_INITIALIZE_IN_PROGRESS to indicate initialization is
   active.  */

THREAD_DECLARE volatile ULONG  ace_os_thread_system_state;

/* Define the array of thread pointers.  Each entry represents the threads that
   are ready at that priority group.  For example, index 10 in this array
   represents the first thread ready at priority 10.  If this entry is NULL,
   no threads are ready at that priority.  */

THREAD_DECLARE ACE_OS_THREAD     *ace_os_thread_priority_list[ACE_OS_MAX_PRIORITIES];

THREAD_DECLARE ULONG             ace_os_thread_priority_maps[ACE_OS_MAX_PRIORITIES/32];

THREAD_DECLARE ULONG             ace_os_thread_preempt_maps[ACE_OS_MAX_PRIORITIES/32];

THREAD_DECLARE UINT              ace_os_thread_highest_priority;

#ifdef __cplusplus
}
#endif
#endif
