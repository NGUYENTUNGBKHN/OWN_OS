#include <stdio.h>
#include "stm32f746.h"
#include "ace_os.h"

#define APP_CFG_TASK_START_STK_SIZE 256

static  ace_os_tcb   Task1_TCB;
static  uint32_t  Task1_StartStk[APP_CFG_TASK_START_STK_SIZE];

static  ace_os_tcb   Task2_TCB;
static  uint32_t  Task2_StartStk[APP_CFG_TASK_START_STK_SIZE];

void task1()
{
    while (1)
    {
        /* code */
    }
    
}

void task2()
{
    while (1)
    {
        /* code */
    }
    
}

int main(void)
{
    ace_os_err err;
    printf("ACE_OS Begin\n");

    ace_os_init(&err);

    ace_os_task_create(&Task1_TCB, task1, 0, &Task1_StartStk[0], 256, 0, &err);
    ace_os_task_create(&Task2_TCB, task2, 0, &Task2_StartStk[0], 256, 0, &err);

    SysTick_Enable(100);

    ace_os_start(&err);

    for (;;) {
    }
}
