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



uint32_t *ace_os_task_stack_init(ace_os_task_func p_task_func,
                                void             *p_arg,
                                uint32_t         *p_stk_base,
                                uint32_t         stk_size,
                                uint32_t         stk_limit)
{
    uint32_t *p_stk;

    p_stk = &p_stk_base[stk_size];

    p_stk = (uint32_t*)((uint32_t)(p_stk) & 0xFFFFFFF8u);
    /* Registers stacked as if auto-saved on exception      */
    *(--p_stk) = (uint32_t)0x01000000u;            /* xPSR                                                 */
    *(--p_stk) = (uint32_t)p_task_func;            /* Entry Point                                          */
    *(--p_stk) = (uint32_t)ace_os_task_return;     /* R14 (LR)                                             */
    *(--p_stk) = (uint32_t)0x12121212u;            /* R12                                                  */
    *(--p_stk) = (uint32_t)0x03030303u;            /* R3                                                   */
    *(--p_stk) = (uint32_t)0x02020202u;            /* R2                                                   */
    *(--p_stk) = (uint32_t)stk_limit;              /* R1                                                   */
    *(--p_stk) = (uint32_t)p_arg;                  /* R0 : argument                                        */
    *(--p_stk) = (uint32_t)THREAD_PSP;             /* R14: EXEC_RETURN; See Note 5                         */
    /* Remaining registers saved on process stack           */                                                           
    *(--p_stk) = (uint32_t)0x11111111uL;           /* R11                                                  */
    *(--p_stk) = (uint32_t)0x10101010uL;           /* R10                                                  */
    *(--p_stk) = (uint32_t)0x09090909uL;           /* R9                                                   */
    *(--p_stk) = (uint32_t)0x08080808uL;           /* R8                                                   */
    *(--p_stk) = (uint32_t)0x07070707uL;           /* R7                                                   */
    *(--p_stk) = (uint32_t)0x06060606uL;           /* R6                                                   */
    *(--p_stk) = (uint32_t)0x05050505uL;           /* R5                                                   */
    *(--p_stk) = (uint32_t)0x04040404uL;           /* R4                                                   */
    
    
    return p_stk;
}






/******************************** End of file *********************************/

