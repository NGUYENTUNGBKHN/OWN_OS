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
#include "stm32f746.h"
#include "thread.h"
#include "uart_debug.h"

static void delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

/**
 * @brief      User task function
 * 
*/
void usertask1(void *data)
{
    printf("User task 1\n");
    while(1)
    {
        printf("User task 1: %s\n", (const char *)data);
        delay(10);
    }
}

void usertask2(void *data)
{
    printf("User task 2\n");

    while(1)
    {
        printf("User task 2: %s\n", (const char *)data);
        delay(10);
    }
}


int main(void)
{
    const char *str1 = "Task1", *str2 = "Task2";

    UART1_Debug_Init();
    printf("ACE OS Lab 5!\n");

    
    thread_create(usertask1, (void *)str1);
    thread_create(usertask2, (void *)str2);
    SysTick_Enable(100);
    thread_start();
   
    while(1) 
    {

    }
}
