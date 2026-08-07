/**
 * @file       ace_os_cpu.c
 * @brief      
 * @date       2026/07/01
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
*/
/*******************************************************************************
**                                INCLUDES
*******************************************************************************/
#include "ace_os.h"
/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define THREAD_PSP  0xFFFFFFFDuL

/*******************************************************************************
**                      COMMON VARIABLE DEFINITIONS
*******************************************************************************/


/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/


/*******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/


/*******************************************************************************
**                          FUNCTION DEFINITIONS
*******************************************************************************/



CPU_STK *ace_os_task_stack_init(ace_os_task_func p_task_func,
                                void             *p_arg,
                                CPU_STK         *p_stk_base,
                                CPU_STK_SIZE     stk_size,
                                CPU_STK_SIZE     stk_limit)
{
    CPU_STK *p_stk;

    p_stk = &p_stk_base[stk_size];
    /* Align the stack to 8-byte */
    p_stk = (CPU_STK*)((CPU_STK)(p_stk) & 0xFFFFFFF8u);
    /* Registers stacked as if auto-saved on exception      */
    *(--p_stk) = (CPU_STK)0x01000000u;            /* xPSR                                                 */
    *(--p_stk) = (CPU_STK)p_task_func;            /* Entry Point                                          */
    *(--p_stk) = (CPU_STK)ace_os_task_return;     /* R14 (LR)                                             */
    *(--p_stk) = (CPU_STK)0x12121212u;            /* R12                                                  */
    *(--p_stk) = (CPU_STK)0x03030303u;            /* R3                                                   */
    *(--p_stk) = (CPU_STK)0x02020202u;            /* R2                                                   */
    *(--p_stk) = (CPU_STK)stk_limit;              /* R1                                                   */
    *(--p_stk) = (CPU_STK)p_arg;                  /* R0 : argument                                        */
    *(--p_stk) = (CPU_STK)THREAD_PSP;             /* R14: EXEC_RETURN; See Note 5                         */
    /* Remaining registers saved on process stack           */                                                           
    *(--p_stk) = (CPU_STK)0x11111111uL;           /* R11                                                  */
    *(--p_stk) = (CPU_STK)0x10101010uL;           /* R10                                                  */
    *(--p_stk) = (CPU_STK)0x09090909uL;           /* R9                                                   */
    *(--p_stk) = (CPU_STK)0x08080808uL;           /* R8                                                   */
    *(--p_stk) = (CPU_STK)0x07070707uL;           /* R7                                                   */
    *(--p_stk) = (CPU_STK)0x06060606uL;           /* R6                                                   */
    *(--p_stk) = (CPU_STK)0x05050505uL;           /* R5                                                   */
    *(--p_stk) = (CPU_STK)0x04040404uL;           /* R4                                                   */
    
    
    return p_stk;
}


void ace_os_systick_handler(void)
{
#if ACE_OS_CFG_NONE_PREEMP
    
#else
    ace_os_time_tick();
#endif 
}



/******************************** End of file *********************************/

