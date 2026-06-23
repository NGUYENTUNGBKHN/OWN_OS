/**
 * @file       osKernel.h
 * @brief      
 * @date       2026/06/21
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
*/
#ifndef _OSKERNEL_H_
#define _OSKERNEL_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */
void osKernelLaunch(uint32_t quanta);

void osKernelInit(void);

uint8_t osKernelAddThreads(void(*task0)(void),
                        void(*task1)(void),
                        void(*task2)(void));



#ifdef __cplusplus
}
#endif
#endif

