/*
****************************************************************************************************************
 * @file       ace_os_byte_allocate.c
 * @brief      
 * @date       2026/10/29
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
/***************************************************************************************************************
**                                                   INCLUDES
***************************************************************************************************************/
#include "ace_os_api.h"
#include "ace_os_byte_pool.h"
#include "ace_os_thread.h"
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


UINT ace_os_byte_allocate(ACE_OS_BYTE_POOL *pool_ptr, VOID **memory_ptr, ULONG memory_size, ULONG wait_option)
{
    ACE_OS_INTERRUPT_SAVE_AREA

    UINT            status = ACE_OS_SUCCESS;
    UINT            finish;
    UINT            suspended_count;
    UCHAR           *work_ptr;
    ACE_OS_THREAD   *next_thread;
    ACE_OS_THREAD   *previous_thread;
    ACE_OS_THREAD   *thread_ptr;

    /* Round the memory size up to the next size that is ewnely divisible by
    an ALIGN_TYPE (this is typically a 32-bit ULONG). This guarantees proper alignment. */
    memory_size = (((memory_size + (sizeof(ALIGN_TYPE))) - ((ALIGN_TYPE) 1))/(sizeof(ALIGN_TYPE))) * (sizeof(ALIGN_TYPE));

    /* Pickup thread pointer. */
    ACE_OS_THREAD_GET_CURRENT(thread_ptr);

    /* Disable interrupts. */
    ACE_OS_DISABLE

    /* Set the search finished flag to flase */
    finish = ACE_OS_FALSE;

    /* Loop to handle cases where the owner of the pool changed. */
    do
    {
        /* Indicate that this thread is the current owner. */
        pool_ptr->ace_os_byte_pool_owner = thread_ptr;

        /* Restore interrupt */
        ACE_OS_RESTORE

        /* At this point, the executing thread owns the pool and can perform a search
            for free memory. */
        work_ptr = ace_os_byte_pool_search(pool_ptr, memory_size);

        /* Lockout interrupt. */
        ACE_OS_DISABLE

        /* Determine of we are finished. */
        if (work_ptr != ACE_OS_NULL)
        {
            /* Yes, we have found a block the search is finished. */
            finish = ACE_OS_TRUE;
        }
        else
        {
            /* No block was found, does this thread still own the pool. */
            if (pool_ptr->ace_os_byte_pool_owner == thread_ptr)
            {
                /* Yes, then we have looked through the entire pool and haven't found the memory. */
                finish = ACE_OS_TRUE;
            }
        }

    }while(!finish);
    
    /* Copy the pointer into the return destination. */
    *memory_ptr = (VOID *) work_ptr;

    /* Determine if memory was found. */
    if (work_ptr != ACE_OS_NULL)
    {

        /* Restore interrupts. */
        ACE_OS_RESTORE

        /* Set the status to success. */
        status = ACE_OS_SUCCESS;
    }
    else
    {
        /* No memory of sufficient size was found .. */

        /* Determine if the request specifies suspension. */
        if (wait_option != ACE_OS_NO_WAIT)
        {
            /* Determine if the preempt disable flag is non-zero. */
            if (ace_os_thread_preempt_disable != ((UINT) 0))
            {
                /* Suspenion is not allowed if the preempt disable flag is non-zero at this point - 
                    return error completion */
                status = ACE_OS_NO_MEMORY;

                /* Restore interrupts. */
                ACE_OS_RESTORE
            }
            else
            {
                /* Prepare for suspenion of this thread. */

                /* Setup cleanup routine pointer */
                thread_ptr->ace_os_thread_suspend_cleanup = &(ace_os_byte_pool_cleanup);

                /* Setup cleanup information, i.e. this pool control
                    block. */
                thread_ptr->ace_os_thread_suspend_control_block = (VOID*)pool_ptr;

                /* Save the return memory pointer address as well. */
                thread_ptr->ace_os_thread_additional_suspend_info = (VOID*)memory_ptr;

                /* Save the byte size requested */
                thread_ptr->ace_os_thread_suspend_info = memory_size;

                /* Increment the suspension sequence number, which is used to identify
                    this suspension event. */
                thread_ptr->ace_os_thread_suspension_sequence ++;

                /* Pickup the number of suspend threads */
                suspended_count = pool_ptr->ace_os_byte_pool_suspended_count;

                /* Increment the suspension count. */
                (pool_ptr->ace_os_byte_pool_suspended_count)++;

                /* Setup suspension list. */
                if (suspended_count == ACE_OS_NO_SUSPENSIONS)
                {
                    /* No other threads are suspended. Setup the head pointer and
                        just setup this theads pointer to itself. */
                    pool_ptr->ace_os_byte_pool_suspension_list = thread_ptr;
                    thread_ptr->ace_os_thread_suspended_next = thread_ptr;
                    thread_ptr->ace_os_thread_suspended_previous = thread_ptr;
                }
                else
                {
                    /* This list is not NULL, and current thread to the end. */
                    next_thread =                                   pool_ptr->ace_os_byte_pool_suspension_list;
                    thread_ptr->ace_os_thread_suspended_next =      next_thread;
                    previous_thread =                               next_thread->ace_os_thread_suspended_previous;
                    thread_ptr->ace_os_thread_suspended_previous =  previous_thread;
                    previous_thread->ace_os_thread_suspended_next = thread_ptr;
                    next_thread->ace_os_thread_suspended_previous = thread_ptr;
                }

                /* Set the state to suspended */
                thread_ptr->ace_os_thread_state = ACE_OS_BYTE_MEMORY;

                /* Set the suspending flag. */
                thread_ptr->ace_os_thread_suspending = ACE_OS_TRUE;

                /* Setup the timeout period. */
                // thread_ptr->ace_os_thread_timer.ace_os_timer_internal_remaining_ticks = wait_option;

                /* Temporarily disable preemption. */
                ace_os_thread_preempt_disable++;

                /* Restore interrupts */
                ACE_OS_RESTORE

                /* Call actual thead suspension routine. */
                ace_os_thread_system_suspend(thread_ptr);

                status = thread_ptr->ace_os_thread_suspend_status;
            }
        }
        else
        {
            /* Restore interrupts. */
            ACE_OS_RESTORE

            /* Set the status to success. */
            status = ACE_OS_NO_MEMORY;
        }
    }

    return status;
}







/************************************************ End of file *************************************************/

