/**
 * @file       ace_os_time.c
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

void ace_os_systick_handler(void)
{
#if ACE_OS_CFG_NONE_PREEMP
    
#else
    if (ace_os_curr_ptr != (ace_os_tcb *)0)
    {
        if (ace_os_curr_ptr->TimeQuantaCtr > 0)
        {
            ace_os_curr_ptr->TimeQuantaCtr--;
        }

        if (ace_os_curr_ptr->TimeQuantaCtr == 0)
        {
            /* Reset the remaining time slice */
            ace_os_curr_ptr->TimeQuantaCtr = ace_os_curr_ptr->TimeQuanta;

            /* Rotate: remove from head and insert at tail */
            ace_os_rdylist_remove(ace_os_curr_ptr);
            ace_os_rdylist_insert_tail(ace_os_curr_ptr);

            /* Call scheduler to switch task */
            ace_os_scheduler();
        }
    }
#endif 
}


/******************************** End of file *********************************/


