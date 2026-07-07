#include <stdio.h>
#include "stm32f746.h"
#include "ace_os.h"

#define APP_CFG_TASK_START_STK_SIZE 256

static  ace_os_tcb   Task1_TCB;
static  uint32_t  Task1_StartStk[APP_CFG_TASK_START_STK_SIZE];

static  ace_os_tcb   Task2_TCB;
static  uint32_t  Task2_StartStk[APP_CFG_TASK_START_STK_SIZE];

volatile uint32_t task1_ctr = 0;
void task1(void *name_tsk)
{
    while (1)
    {
        printf("%s \n", (char*)name_tsk);
        task1_ctr++;
        ace_os_yield();
    }
    
}

volatile uint32_t task2_ctr = 0;
void task2(void *name_tsk)
{
    while (1)
    {
        printf("%s \n", (char*)name_tsk);
        task2_ctr++;
        ace_os_yield();
    }
    
}

int main(void)
{
    char *task1_name = "task1";
    char *task2_name = "task2";
    ace_os_err err;
    printf("ACE_OS Begin\n");

    ace_os_init(&err);

    ace_os_task_create(&Task1_TCB, task1, task1_name, &Task1_StartStk[0], 256, 0, &err);
    ace_os_task_create(&Task2_TCB, task2, task2_name, &Task2_StartStk[0], 256, 0, &err);

    SysTick_Enable(100);

    ace_os_start(&err);

    for (;;) {
    }
}
