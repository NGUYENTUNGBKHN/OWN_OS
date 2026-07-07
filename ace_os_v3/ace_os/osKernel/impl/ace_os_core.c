/**
 * @file       ace_os_core.c
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
ace_os_tcb *ace_os_curr_ptr;
ace_os_tcb *ace_os_next_ptr;
ace_os_rdy_list AceOSRdyList[32];
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

void ace_os_init(ace_os_err *p_err)
{
    ace_os_curr_ptr = 0;

    ace_os_rdylist_init();

    ace_os_task_init(p_err);
    if (*p_err != ACE_OS_ERR_NONE)
    {
        return;
    }

    *p_err = ACE_OS_ERR_NONE;
}

void ace_os_scheduler()
{
    if (ace_os_curr_ptr == (ace_os_tcb *)0)
    {
        return;
    }

    ace_os_tcb *p_next_tcb = AceOSRdyList[0].HeadPtr;

    if (p_next_tcb != ace_os_curr_ptr)
    {
        ace_os_next_ptr = p_next_tcb;
        ace_os_ctxSw();
    }
}

void ace_os_start(ace_os_err *p_err)
{
    ace_os_next_ptr = AceOSRdyList[0].HeadPtr;
    ace_os_curr_ptr = AceOSRdyList[0].HeadPtr;

    ace_os_start_rdy();
    *p_err = ACE_OS_ERR_NONE;
}

void ace_os_rdylist_init()
{
    ace_os_rdy_list *p_rdy_list;

    for (uint32_t i = 0; i < 32; i++)
    {
        p_rdy_list = &AceOSRdyList[i];

        p_rdy_list->HeadPtr = 0;
        p_rdy_list->TailPtr = 0;
    }
}

void ace_os_rdylist_insert(ace_os_tcb *p_tcb)
{
    (void)p_tcb;
}

void ace_os_rdylist_insert_head(ace_os_tcb *p_tcb)
{
    (void)p_tcb;
}

void ace_os_rdylist_insert_tail(ace_os_tcb *p_tcb)
{
    ace_os_rdy_list *p_rdy_list;
    ace_os_tcb      *p_tcb2;

    p_rdy_list = &AceOSRdyList[0];
    if (p_rdy_list->HeadPtr == 0)
    {
        p_tcb->NextPtr      = 0;
        p_tcb->PrevPtr      = 0;
        p_rdy_list->HeadPtr = p_tcb;
        p_rdy_list->TailPtr = p_tcb;
    }
    else
    {
        p_tcb->NextPtr      = 0;
        p_tcb2              = p_rdy_list->TailPtr;
        p_tcb->PrevPtr      = p_tcb2;
        p_tcb2->NextPtr     = p_tcb;
        p_rdy_list->TailPtr = p_tcb;
    }
}

void ace_os_rdylist_remove(ace_os_tcb *p_tcb)
{
    ace_os_rdy_list *p_rdy_list = &AceOSRdyList[0];

    if (p_tcb->PrevPtr != (ace_os_tcb *)0)
    {
        p_tcb->PrevPtr->NextPtr = p_tcb->NextPtr;
    }
    else
    {
        p_rdy_list->HeadPtr = p_tcb->NextPtr;
    }

    if (p_tcb->NextPtr != (ace_os_tcb *)0)
    {
        p_tcb->NextPtr->PrevPtr = p_tcb->PrevPtr;
    }
    else
    {
        p_rdy_list->TailPtr = p_tcb->PrevPtr;
    }

    p_tcb->NextPtr = (ace_os_tcb *)0;
    p_tcb->PrevPtr = (ace_os_tcb *)0;
}
void ace_os_yield(void)
{
    if (ace_os_curr_ptr != (ace_os_tcb *)0)
    {
        /* Reset remaining time slice */
        ace_os_curr_ptr->TimeQuantaCtr = ace_os_curr_ptr->TimeQuanta;

        /* Move current task to the tail of ready list */
        ace_os_rdylist_remove(ace_os_curr_ptr);
        ace_os_rdylist_insert_tail(ace_os_curr_ptr);

        /* Schedule immediately */
        ace_os_scheduler();
    }
}


/******************************** End of file *********************************/

