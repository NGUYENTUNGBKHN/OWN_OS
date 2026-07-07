/**
 * @file       ace_os_cpu.h
 * @brief      
 * @date       2026/06/29
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
*/
#ifndef _ACE_OS_CPU_H_
#define _ACE_OS_CPU_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void ace_os_start_rdy(void);
void ace_os_intCtxSw(void);
void ace_os_ctxSw(void);

// void  ace_os_systick_init    (uint32_t  cnts);
// void  ace_os_systick_init_freq(uint32_t  cpu_freq);

void  ace_os_systick_handler(void);
void  ace_os_pendSV_handler (void);

#ifdef __cplusplus
}
#endif
#endif
