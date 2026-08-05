/*
****************************************************************************************************************
 * @file       ace_os_task.c
 * @brief      
 * @date       2026/07/29
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
/***************************************************************************************************************
**                                                   INCLUDES
***************************************************************************************************************/
#include "ace_os.h"
/***************************************************************************************************************
**                                         EXTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                        EXTERNAL VARIABLE DECLARATIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                          INTERNAL MACRO DEFINITIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                         COMMON VARIABLE DEFINITIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                        INTERNAL VARIABLE DEFINITIONS
***************************************************************************************************************/


/***************************************************************************************************************
**                                         INTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                             FUNCTION DEFINITIONS
***************************************************************************************************************/

/*
****************************************************************************************************************
 *                                          TASK MANAGER INITIALIZATION
 * 
 * @brief      Initilize the task management
 * 
 * @param      p_err    is a pointer to a variable that will contain an error code returned by this function.
 *                      
 *                      ACE_OS_ERR_NONE   the call was successful 
****************************************************************************************************************
*/
void ace_os_task_init(ace_os_err *p_err)
{

    *p_err = ACE_OS_ERR_NONE;
}

/*
****************************************************************************************************************
 *                                          TASK CONTROLL BLOCK INITIALIZATION
 * 
 * @brief      Initialize the task control block 
 * 
 * @param      p_tcb    is a pointer to the TCB to initialize
 * 
****************************************************************************************************************
*/
void ace_os_task_init_tcb(ace_os_tcb *p_tcb)
{
    /* Init current top of stack pointer */
    p_tcb->StkPtr = 0;
    
    /* Init previous + next TCB pointer */
    p_tcb->PrevPtr = 0;
    p_tcb->NextPtr = 0;

    /* Init base address of stack pointer */
    p_tcb->StkBasePtr = 0;
    p_tcb->Stk_size = 0;

    /* Init task entry pointer */
    p_tcb->TaskEntryAddr = 0;
    p_tcb->TaskEntryArg = 0;

    /* Init time slice */
    p_tcb->TimeQuanta = 0;
    p_tcb->TimeQuantaCtr = 0;

    /* Init State -> Ready */
    p_tcb->TaskState = ACE_OS_TASK_STATE_RDY;

    /* Init priority  */
    p_tcb->Prio = 0;
}

/*
****************************************************************************************************************
 *                                      CREATE TASK
 * 
 * @brief      
 * @param      p_tcb        : Pointer task control block
 * @param      p_task_func  : Entry task function
 * @param      p_arg        : Argument of task function
 * @param      prio         : Task priority
 * @param      p_stk_base   : Stack base address
 * @param      stk_size     : Size of Stack
 * @param      stk_limit    : 
 * @param      p_err        : Return code
 * 
****************************************************************************************************************
*/
void ace_os_task_create(ace_os_tcb          *p_tcb,
                        ace_os_task_func    p_task_func,
                        void                *p_arg,
                        uint32_t            prio,
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

    /* Init stack of task */
    p_sp = ace_os_task_stack_init(p_task_func, p_arg, p_stk_base, stk_size, stk_limit);    

    /* ------------ SETUP THE TCB FIELDS ------------- */
    p_tcb->TaskEntryAddr = p_task_func;
    p_tcb->TaskEntryArg = p_arg;

    p_tcb->Prio = prio;
    // p_sp = p_stk_base;
    p_tcb->StkPtr = p_sp;

    p_tcb->StkBasePtr = p_stk_base;
    p_tcb->Stk_size = stk_size;

    p_tcb->TimeQuanta = ACE_OS_CFG_TIME_QUANTA;
    p_tcb->TimeQuantaCtr = ACE_OS_CFG_TIME_QUANTA;

    /* -------------- ADD TASK TO READY LIST -------------- */
    ace_os_prio_insert(p_tcb->Prio);
    ace_os_rdylist_insert_tail(p_tcb);

    ace_os_scheduler();
}

void ace_os_task_del(ace_os_tcb *p_tcb,
                        ace_os_err *p_err)
{
    
    if (p_tcb == ACE_NULL)
    {
        p_tcb = ace_os_tcb_curr_ptr;
    }

    switch (p_tcb->TaskState)
    {
    case ACE_OS_TASK_STATE_RDY:
        ace_os_rdylist_remove(p_tcb);
        break;
    case ACE_OS_TASK_STATE_SUSPENDED:
        break;
    case ACE_OS_TASK_STATE_DLY:
    case ACE_OS_TASK_STATE_DLY_SUSPENDED:
        ace_os_rdylist_remove(p_tcb);
        break;
    case ACE_OS_TASK_STATE_PEND:
    case ACE_OS_TASK_STATE_PEND_SUSPENDED:
    case ACE_OS_TASK_STATE_PEND_TIMEOUT:
    case ACE_OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
        break;
    default:
        *p_err = ACE_OS_TCB_INVALID;
        break;
    }

    ace_os_task_init_tcb(p_tcb);
    *p_err = ACE_OS_ERR_NONE;
    ace_os_scheduler();
}

void ace_os_task_suspend(ace_os_tcb *p_tcb,
                    ace_os_err *p_err)
{
    if (p_tcb == ACE_NULL)
    {
        p_tcb = ace_os_tcb_curr_ptr;
    }

    *p_err = ACE_OS_ERR_NONE;
}

void ace_os_task_return()
{
    ace_os_err err;

    ace_os_task_del((ace_os_tcb*) 0,
                    &err);
}

/************************************************ End of file *************************************************/
