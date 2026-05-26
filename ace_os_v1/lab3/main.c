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

    }
}

void usertask2(void)
{
    printf("User task 2\n");
    syscall();
    while(1)
    {

    }
}

int main(void)
{
    // Simulate user task stack (not used in this simple example)
    unsigned int usertask_stack[256];
    // Point to the top of the user task stack
    unsigned int *usertask_stack_start = usertask_stack + 256 - 16;
    usertask_stack_start[8] = (unsigned int)usertask1; // Set the return address to usertask

    printf("ACE OS Lab 3!\n");
    activate(usertask_stack_start); // Start the user task
    printf("Back to main\n");
    
    while(1) 
    {

    }
}
