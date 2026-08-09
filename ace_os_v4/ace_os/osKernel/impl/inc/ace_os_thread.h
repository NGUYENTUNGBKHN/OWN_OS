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

+void ace_os_thread_create(void);
void ace_os_thread_delete(void);
void ace_os_thread_entry_exit_notify(void);
void ace_os_thread_identify(void);
void ace_os_thread_info_get(void);
void ace_os_thread_initialize(void);
void ace_os_thread_performance_info_get(void);
void ace_os_thread_performance_system_info_get(void);
void ace_os_thread_preemption_change(void);
void ace_os_thread_priority_change(void);
void ace_os_thread_relinquish(void);
void ace_os_thread_reset(void);
void ace_os_thread_resume(void);
void ace_os_thread_shell_entry(void);
void ace_os_thread_sleep(void);
void ace_os_thread_stack_analyze(void);
void ace_os_thread_stack_error_handler(void);
void ace_os_thread_stack_error_notify(void);
void ace_os_thread_suspend(void);
void ace_os_thread_system_preempt_check(void);
void ace_os_thread_system_resume(void);
void ace_os_thread_system_suspend(void);
void ace_os_thread_terminate(void);
void ace_os_thread_time_slice_change(void);
void ace_os_thread_time_slice(void);
void ace_os_thread_timeout(void);
void ace_os_thread_wait_abort(void);

#ifdef __cplusplus
}
#endif
#endif
