/**
 * @file       ace_os_prio.c
 * @brief      
 * @date       2026/07/07
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
#define ACE_OS_PRIO_TBL_SIZE 1

/*******************************************************************************
**                      COMMON VARIABLE DEFINITIONS
*******************************************************************************/


/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
CPU_DATA AceOSPrioTbl[ACE_OS_PRIO_TBL_SIZE];

/*******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/


/*******************************************************************************
**                          FUNCTION DEFINITIONS
*******************************************************************************/
void ace_os_prio_init()
{
    for(CPU_DATA i = 0; i < ACE_OS_PRIO_TBL_SIZE; i++)
    {
        AceOSPrioTbl[i] = 0;
    }
}


ACE_OS_PRIO ace_os_prio_get_highest()
{
    ACE_OS_PRIO prio;
    CPU_DATA *p_tbl;

    prio = 0;
    p_tbl = &AceOSPrioTbl[0];

    prio += (ACE_OS_PRIO)CPU_CntLeadZeros(*p_tbl);

    return prio;
}

void ace_os_prio_insert(ACE_OS_PRIO prio)
{
    CPU_DATA bit;
    CPU_DATA bit_nbr;
    ACE_OS_PRIO ix;

    ix = 0;
    bit_nbr = prio;
    bit  = 1;
    bit  <<= (32 - 1) - bit_nbr;
    AceOSPrioTbl[ix] |= bit;
}

void ace_os_prio_remove(ACE_OS_PRIO prio)
{
    CPU_DATA bit;
    CPU_DATA bit_nbr;
    ACE_OS_PRIO ix;

    ix = 0;
    bit_nbr = prio;
    bit  = 1;
    bit  <<= (32 - 1) - bit_nbr;
    AceOSPrioTbl[ix] &= ~bit;
}

/******************************** End of file *********************************/

