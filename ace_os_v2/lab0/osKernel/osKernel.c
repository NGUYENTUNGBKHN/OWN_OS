/**
 * @file       osKernel.c
 * @brief      
 * @date       2026/06/21
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
*/
#include <stdint.h>
#include "stm32f746.h"
#include "osKernel.h"

#define NUM_OF_THREADS      3
#define STACKSIZE           100
#define BUS_FREQ            16000000

struct tcb_s
{
    int32_t *stackPt;
    struct tcb_s *nextPt;
};

typedef struct tcb_s tcbType;
tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void osKernelStackInit(int i)
{
    tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE - 16];
    TCB_STACK[i][STACKSIZE - 1] = 0x01000000;
}

void osKernelLaunch(uint32_t quanta)
{
    (void)quanta;

}

void osKernelInit(void)
{

}

uint8_t osKernelAddThreads(void(*task0)(void),
                        void(*task1)(void),
                        void(*task2)(void))
{
    __disable_irq();

    tcbs[0].nextPt = &tcbs[1];
    tcbs[1].nextPt = &tcbs[2];
    tcbs[2].nextPt = &tcbs[0];

    osKernelStackInit(0);
    TCB_STACK[0][STACKSIZE - 2] = (int32_t)task0;

    osKernelStackInit(1);
    TCB_STACK[1][STACKSIZE - 2] = (int32_t)task1;

    osKernelStackInit(2);
    TCB_STACK[2][STACKSIZE - 2] = (int32_t)task2;

    __enable_irq();
    return 1;
}
