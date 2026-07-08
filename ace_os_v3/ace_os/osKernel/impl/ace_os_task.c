/**
 * @file       ace_os_task.c
 * @brief      
 * @date       2026/06/29
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

void ace_os_task_init(ace_os_err *p_err)
{

    *p_err = ACE_OS_ERR_NONE;
}

void ace_os_task_init_tcb(ace_os_tcb *p_tcb)
{
    p_tcb->StkPtr = 0;
    
    p_tcb->PrevPtr = 0;
    p_tcb->NextPtr = 0;

    p_tcb->StkBasePtr = 0;
    p_tcb->Stk_size = 0;

    p_tcb->TaskEntryAddr = 0;
    p_tcb->TaskEntryArg = 0;

    p_tcb->TimeQuanta = 0;
    p_tcb->TimeQuantaCtr = 0;
}

void ace_os_task_create(ace_os_tcb          *p_tcb,
                        ace_os_task_func    p_task_func,
                        void                *p_arg,
                        uint32_t            *p_stk_base,
                        uint32_t            stk_size,
                        uint32_t            stk_limit,
                        ace_os_err          *p_err)
{
    uint32_t *p_sp;
    // uint32_t *p_stk_limit;
    (void)stk_limit;


    if (p_tcb == 0)
    {
        /*TODO: Add trace OS */
        *p_err = ACE_OS_TCB_INVALID;
        return;
    }

    

    ace_os_task_init_tcb(p_tcb);    /* Initialize the TCB to default values */

    *p_err = ACE_OS_ERR_NONE;

    p_sp = ace_os_task_stack_init(p_task_func, p_arg, p_stk_base, stk_size, stk_limit);    

    /* ------------ SETUP THE TCB FIELDS ------------- */
    p_tcb->TaskEntryAddr = p_task_func;
    p_tcb->TaskEntryArg = p_arg;

    // p_sp = p_stk_base;
    p_tcb->StkPtr = p_sp;

    p_tcb->StkBasePtr = p_stk_base;
    p_tcb->Stk_size = stk_size;

    p_tcb->TimeQuanta = ACE_OS_CFG_TIME_QUANTA;
    p_tcb->TimeQuantaCtr = ACE_OS_CFG_TIME_QUANTA;

    /* -------------- ADD TASK TO READY LIST -------------- */
    ace_os_rdylist_insert_tail(p_tcb);

    ace_os_scheduler();
}

void ace_os_task_del(ace_os_tcb *p_tcb,
                    ace_os_err *p_err)
{
    (void)p_tcb;

    *p_err = ACE_OS_ERR_NONE;
}

void ace_os_task_return()
{
    ace_os_err err;

    ace_os_task_del((ace_os_tcb*) 0,
                    &err);
}


/******************************** End of file *********************************/


