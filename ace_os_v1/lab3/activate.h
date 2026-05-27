/**
 * @file       activate.h
 * @brief      
 * @date       2026/05/26
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
*/
#ifndef ACTIVATE_H
#define ACTIVATE_H  


unsigned int *activate(unsigned int *stack_top);
void syscall(void);
void task_init_env(unsigned int *stack);

#endif /* ACTIVATE_H */
