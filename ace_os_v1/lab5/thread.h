/**
 * @file       thread.h
 * @brief      
 * @date       2026/05/27
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
*/
#ifndef THREAD_H
#define THREAD_H    

#define STACK_SIZE 256

int thread_create(void (*task_func)(void*), void *userdata);
void thread_self_terminal(void);
void thread_start();

#endif /* THREAD_H */
