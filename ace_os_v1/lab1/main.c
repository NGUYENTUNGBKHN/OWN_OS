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
void usertask(void)
{
    printf("User task\n");
    while(1);
}

int main(void)
{
    // Simulate user task stack (not used in this simple example)
    unsigned int usertask_stack[256];
    // Point to the top of the user task stack
    unsigned int *usertask_stack_start = usertask_stack + 256 - 16;
    usertask_stack_start[8] = (unsigned int)usertask; // Set the return address to usertask

    printf("ACE OS Lab 1!\n");
    activate(usertask_stack_start); // Start the user task
    while(1) 
    {

    }
}
