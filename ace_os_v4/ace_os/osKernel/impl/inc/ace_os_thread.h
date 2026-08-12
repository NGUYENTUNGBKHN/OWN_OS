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

UINT ace_os_thread_create(ACE_OS_THREAD *thread_ptr, 
                          CHAR *name_ptr,
                          VOID (*entry_function)(ULONG id),
                          ULONG entry_input,
                          VOID *stack_start,
                          ULONG stack_size,
                          UINT priority,
                          ULONG time_slice,
                          UINT auto_start);
UINT ace_os_thread_delete(VOID);
UINT ace_os_thread_entry_exit_notify(VOID);
ACE_OS_THREAD *ace_os_thread_identify(void);
UINT ace_os_thread_info_get(void);
VOID ace_os_thread_initialize(void);
UINT ace_os_thread_performance_info_get(void);
UINT ace_os_thread_performance_system_info_get(void);
UINT ace_os_thread_preemption_change(void);
UINT ace_os_thread_priority_change(void);
UINT ace_os_thread_relinquish(void);
UINT ace_os_thread_reset(void);
UINT ace_os_thread_resume(void);

VOID ace_os_thread_shell_entry(ACE_OS_THREAD *thread_ptr, VOID (*function_ptr)(VOID));

UINT ace_os_thread_sleep(void);
VOID ace_os_thread_stack_analyze(void);
VOID ace_os_thread_stack_error_handler(void);
UINT ace_os_thread_stack_error_notify(void);
UINT ace_os_thread_suspend(void);
VOID ace_os_thread_system_preempt_check(void);
VOID ace_os_thread_system_resume(void);
VOID ace_os_thread_system_suspend(void);
UINT ace_os_thread_terminate(void);
UINT ace_os_thread_time_slice_change(void);
VOID ace_os_thread_time_slice(void);
VOID ace_os_thread_timeout(void);
UINT ace_os_thread_wait_abort(void);

#define ACE_OS_DECLARE  extern

/* Define the head pointer of the created thread list.  */

ACE_OS_DECLARE ACE_OS_THREAD * ace_os_thread_created_ptr;

/* Define the variable that holds the number of created threads. */

ACE_OS_DECLARE ULONG ace_os_thread_created_count;


/* Define the current state variable.  When this value is 0, a thread
   is executing or the system is idle.  Other values indicate that
   interrupt or initialization processing is active.  This variable is
   initialized to ACE_OS_INITIALIZE_IN_PROGRESS to indicate initialization is
   active.  */

ACE_OS_DECLARE volatile ULONG  ace_os_thread_system_state;

#ifdef __cplusplus
}
#endif
#endif
