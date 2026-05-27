/**
 * @file       main.c
 * @brief      
 * @date       2026/05/26
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
*/
#include <stdio.h>
#include "activate.h"
#include "stm32f746.h"

/* Exception return behavior */
#define HANDLER_MSP	0xFFFFFFF1
#define THREAD_MSP	0xFFFFFFF9
#define THREAD_PSP	0xFFFFFFFD

void task_init()
{
    unsigned int empty[32];
    task_init_env(empty + 32);
}

void delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

/**
 * @brief      User task function
 * 
*/
void usertask1(void)
{
    printf("User task 1\n");
    syscall();
    while(1)
    {
        printf("User task 1\n");
        delay(1000); // Add some delay to simulate work and allow for task switching
    }
}

void usertask2(void)
{
    printf("User task 2\n");
    syscall();
    while(1)
    {
        printf("User task 2\n");
        delay(1000); // Add some delay to simulate work and allow for task switching
    }
}

unsigned int *create_tasks(unsigned int *stack, void (*task_func)(void))
{
    unsigned int *res;
    unsigned int *stack_start = stack + 256 - 17;
    stack_start[8] = THREAD_PSP; // LR (EXC_RETURN to Thread mode, use PSP)
    stack_start[15] = (unsigned int)task_func; // PC
    stack_start[16] = 0x01000000; // xPSR (Thumb state)
    res = activate(stack_start);

    return res;
}

int main(void)
{
    // Simulate user task stack (not used in this simple example)
    unsigned int usertask_stack[2][256];
    unsigned int *usertask_stack_ptr[2]; // Point to the first task's stack
    unsigned int current_task = 0; // Index of the current task

    printf("ACE OS Lab 4!\n");
    task_init();
    
    usertask_stack_ptr[0] = create_tasks(usertask_stack[0], usertask1);
    usertask_stack_ptr[1] = create_tasks(usertask_stack[1], usertask2);
    SysTick_Enable(1000);
    
    while(1) 
    {
        printf("Switching to user task %d\n", current_task + 1);
        usertask_stack_ptr[current_task] = activate(usertask_stack_ptr[current_task]); // Activate user task 1
        printf("Back to main task\n");
        current_task = (current_task + 1) % 2; // Switch to the next task
    }
}
