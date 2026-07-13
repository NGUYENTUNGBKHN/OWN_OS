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
ace_os_tcb *ace_os_tcb_curr_ptr;
ace_os_tcb *ace_os_high_rdy_ptr;
ace_os_rdy_list AceOSRdyList[32];

uint8_t ace_os_prio_curr;
uint8_t ace_os_prio_high_rdy;
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
    ace_os_tcb_curr_ptr     = ACE_NULL;
    ace_os_high_rdy_ptr = ACE_NULL;

    ace_os_prio_curr        = 0;
    ace_os_prio_high_rdy    = 0;

    ace_os_prio_init();

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
    if (ace_os_tcb_curr_ptr == (ace_os_tcb *)ACE_NULL)
    {
        return;
    }
    ace_os_prio_high_rdy    = ace_os_prio_get_highest();
    ace_os_tcb *p_next_tcb  = AceOSRdyList[ace_os_prio_high_rdy].HeadPtr;

    if (p_next_tcb != ace_os_tcb_curr_ptr)
    {
        ace_os_high_rdy_ptr = p_next_tcb;
        ACE_OS_TASK_SW();                   /* Perform a task level context switch */
    }

}

void ace_os_start(ace_os_err *p_err)
{
    ace_os_prio_high_rdy    = ace_os_prio_get_highest();
    ace_os_prio_curr        = ace_os_prio_high_rdy;
    ace_os_high_rdy_ptr     = AceOSRdyList[ace_os_prio_high_rdy].HeadPtr;
    ace_os_tcb_curr_ptr         = ace_os_high_rdy_ptr;

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
    ace_os_prio_insert(p_tcb->Prio);
    if (p_tcb->Prio == ace_os_prio_curr)
    {
        ace_os_rdylist_insert_tail(p_tcb);
    }
    else
    {
        ace_os_rdylist_insert_head(p_tcb);
    }
}

void ace_os_rdylist_insert_head(ace_os_tcb *p_tcb)
{
    ace_os_rdy_list *p_rdy_list;
    ace_os_tcb      *p_tcb2;

    p_rdy_list = &AceOSRdyList[p_tcb->Prio];
    if (p_rdy_list->HeadPtr == ACE_NULL)
    {
        p_rdy_list->NbrEntries  = 1;

        p_tcb->NextPtr          = ACE_NULL;
        p_tcb->PrevPtr          = ACE_NULL;
        p_rdy_list->HeadPtr     = p_tcb;
        p_rdy_list->TailPtr     = p_tcb;
    }
    else
    {
        p_rdy_list->NbrEntries ++;

        p_tcb->NextPtr      = p_rdy_list->HeadPtr;
        p_tcb->PrevPtr      = ACE_NULL;
        p_tcb2              = p_rdy_list->HeadPtr;
        p_tcb2->PrevPtr     = p_tcb;
        p_rdy_list->HeadPtr = p_tcb;
    }
}

void ace_os_rdylist_insert_tail(ace_os_tcb *p_tcb)
{
    ace_os_rdy_list *p_rdy_list;
    ace_os_tcb      *p_tcb2;

    p_rdy_list = &AceOSRdyList[p_tcb->Prio];
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

void ace_os_rdylist_move_head_to_tail(ace_os_rdy_list *p_rdy_list)
{
    ace_os_tcb  *p_tcb1;
    ace_os_tcb  *p_tcb2;
    ace_os_tcb  *p_tcb3;

    if (p_rdy_list->HeadPtr != p_rdy_list->TailPtr)
    {
        if (p_rdy_list->HeadPtr->NextPtr == p_rdy_list->TailPtr)
        {
            p_tcb1              = p_rdy_list->HeadPtr;
            p_tcb2              = p_rdy_list->TailPtr;
            p_tcb1->PrevPtr     = p_tcb2;
            p_tcb1->NextPtr     = ACE_NULL;
            p_tcb2->PrevPtr     = ACE_NULL;
            p_tcb2->NextPtr     = p_tcb1;
            p_rdy_list->HeadPtr = p_tcb2;
            p_rdy_list->TailPtr = p_tcb1;
        }
        else
        {
            p_tcb1              = p_rdy_list->HeadPtr;
            p_tcb2              = p_rdy_list->TailPtr;
            p_tcb3              = p_tcb1->NextPtr;
            p_tcb3->PrevPtr     = ACE_NULL;
            p_tcb1->NextPtr     = ACE_NULL;
            p_tcb1->PrevPtr     = p_tcb2;
            p_tcb2->NextPtr     = p_tcb1;
            p_rdy_list->HeadPtr = p_tcb3;
            p_rdy_list->TailPtr = p_tcb1;
        }
    }

}

void ace_os_rdylist_remove(ace_os_tcb *p_tcb)
{
    ace_os_rdy_list *p_rdy_list;
    ace_os_tcb      *p_tcb1;
    ace_os_tcb      *p_tcb2;

    p_rdy_list  = &AceOSRdyList[p_tcb->Prio];
    p_tcb1      = p_tcb->PrevPtr;
    p_tcb2      = p_tcb->NextPtr;

    if (p_tcb1 == ACE_NULL)
    {
        if (p_tcb2 == ACE_NULL)
        {
            p_rdy_list->NbrEntries  = ACE_NULL;
            p_rdy_list->HeadPtr     = ACE_NULL;
            p_rdy_list->TailPtr     = ACE_NULL;
        }
        else
        {
            p_rdy_list->NbrEntries --;
            p_tcb2->PrevPtr     = ACE_NULL;
            p_rdy_list->HeadPtr = p_tcb2;
        }
    }
    else        // p_tcb1 != NULL
    {
        p_rdy_list->NbrEntries --;
        p_tcb1->NextPtr = p_tcb2;
        if (p_tcb2 == ACE_NULL)
        {
            p_rdy_list->TailPtr = p_tcb1;
        }
        else
        {   
            p_tcb2->PrevPtr = p_tcb1;
        }
    }

    p_tcb->NextPtr = (ace_os_tcb *)0;
    p_tcb->PrevPtr = (ace_os_tcb *)0;
    /*TODO: TRACE */
}
void ace_os_yield(void)
{
    if (ace_os_tcb_curr_ptr != (ace_os_tcb *)0)
    {
        /* Reset remaining time slice */
        ace_os_tcb_curr_ptr->TimeQuantaCtr = ace_os_tcb_curr_ptr->TimeQuanta;

        /* Move current task to the tail of ready list */
        ace_os_rdylist_move_head_to_tail(AceOSRdyList);

        /* Schedule immediately */
        ace_os_scheduler();
    }
}


/******************************** End of file *********************************/

