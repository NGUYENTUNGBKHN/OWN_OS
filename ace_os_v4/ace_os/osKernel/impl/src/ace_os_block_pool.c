/*
****************************************************************************************************************
 * @file       ace_os_block_pool.c
 * @brief      
 * @date       2026/08/09
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
#include "ace_os_block_pool.h"
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
/* Locate block pool component data in this file.  */

/* Define the head pointer of the created block pool list.  */
ACE_OS_BLOCK_POOL *  ace_os_block_pool_created_ptr;


/* Define the variable that holds the number of created block pools. */
ULONG            ace_os_block_pool_created_count;

/***************************************************************************************************************
**                                         INTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                             FUNCTION DEFINITIONS
***************************************************************************************************************/

/*
****************************************************************************************************************
 *                                      ALLOCATE BLOCK POOL
 * @brief      
 * @param      pool_ptr    
 * @param      block_ptr    
 * @param      wait_option    
 * @return     UINT
****************************************************************************************************************
*/
UINT ace_os_block_allocate(ACE_OS_BLOCK_POOL *pool_ptr, VOID **block_ptr, ULONG wait_option)
{
    ACE_OS_INTERRUPT_SAVE_AREA

    UINT    status;
    UCHAR   *work_ptr;
    UCHAR   *temp_ptr;
    UCHAR   **return_ptr;
    UCHAR   **next_block_ptr;

    /* Disable interrupts to get a block from the pool.  */
    ACE_OS_DISABLE

    /* Determine if there is an available block. */
    if (pool_ptr->ace_os_block_pool_available != ((UINT)0))
    {
        /* Decrement the available count. */
        pool_ptr->ace_os_block_pool_available --;

        /* Pickup the current block pointer. */
        work_ptr = pool_ptr->ace_os_block_pool_available_list;

        /* Return the first available block to the caller. */
        temp_ptr = ACE_OS_UCHAR_POINTER_ADD(work_ptr, (sizeof(UCHAR*)));
        return_ptr = ACE_OS_INDIRECT_VOID_TO_UCHAR_POINTER_CONVERT(block_ptr);
        *return_ptr = temp_ptr;

        /* Modify the available list to point at the next block in the pool. */
        next_block_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(work_ptr);
        pool_ptr->ace_os_block_pool_available_list = *next_block_ptr;

        /* Save the pool's address in the block for when it is released! */
        temp_ptr = ACE_OS_BLOCK_POOL_TO_UCHAR_POINTER_CONVERT(pool_ptr);
        *next_block_ptr = temp_ptr;

        /* Set status to success. */
        status = ACE_OS_SUCCESS;

        /* Restore */
        ACE_OS_RESTORE
    }
    else
    {

        if (wait_option != ACE_OS_NO_WAIT)
        {

        }
        else
        {
            /* Immediate return, return error completion. */
            status = ACE_OS_NO_MEMORY;

            /* Restore */
            ACE_OS_RESTORE
        }
    }

    return status;
}

VOID ace_os_block_pool_clenup()
{

}

/*
****************************************************************************************************************
 *                                      CREATE BLOCK POOL
 *  
 * @brief       Thí function creates a pool of fixed-size memory blocks in the specified memory area.      
 * @param      pool_ptr         Pointer to pool control block
 * @param      name_ptr    
 * @param      block_size    
 * @param      pool_start    
 * @param      pool_size    
 * @return     UINT
 * 
****************************************************************************************************************
*/
UINT ace_os_block_pool_create(ACE_OS_BLOCK_POOL *pool_ptr, CHAR *name_ptr, ULONG block_size,
                            VOID *pool_start, ULONG pool_size)
{
    ACE_OS_INTERRUPT_SAVE_AREA
    UINT    status;
    UINT    blocks;
    UCHAR   *block_ptr;
    UCHAR   **block_link_ptr;
    UCHAR   *next_block_ptr;
    ULONG   total_blocks;

    ACE_OS_BLOCK_POOL *next_pool;
    ACE_OS_BLOCK_POOL *prev_pool;

    /* Initialize block pool control block to all zeros. */
    ACE_OS_MEMSET(pool_ptr, 0, (sizeof(ACE_OS_BLOCK_POOL)));

    /* Round the block size up to something that is evenly divisiable by
       An ALIGN_TYPE (typically this is a 32-bit ULONG). This helps guarantee proper aligment. */
    block_size = (((block_size + (sizeof(ALIGN_TYPE))) - ((ALIGN_TYPE)1))/(sizeof(ALIGN_TYPE)) * sizeof(ULONG));

    /* Round the pool size down to something that is evenly divisible by 
        an ALIGN_TYPE (typically this is a 32-bit ULONG). */
    pool_size = (pool_size/(sizeof(ALIGN_TYPE)))* (sizeof(ALIGN_TYPE));

    /* Setup the basic block pool field */
    pool_ptr->ace_os_block_pool_name        = name_ptr;
    pool_ptr->ace_os_block_pool_start       = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);
    pool_ptr->ace_os_block_pool_size        = pool_size;
    pool_ptr->ace_os_block_pool_block_size  = (UINT) block_size;
    
    /* Calculate the total number of blocks. */
    total_blocks = pool_size/(block_size + (sizeof(UCHAR*)));

    /* Walk through the pool area, setting up the avaiable block list. */
    blocks = ((UINT)0);
    block_ptr = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);
    next_block_ptr = ACE_OS_UCHAR_POINTER_ADD(block_ptr, (block_size + (sizeof(UCHAR*))));
    while(blocks < (UINT) total_blocks)
    {
        /* Increment the block count. */
        blocks ++;

        /* Setup link the next block. */
        block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(block_ptr);
        *block_link_ptr = next_block_ptr;

        /* Advance to the next block. */
        block_ptr = next_block_ptr;

        /* Update the next block pointer. */
        next_block_ptr = ACE_OS_UCHAR_POINTER_ADD(block_ptr, (block_size + (sizeof(UCHAR*))));
    }

    /* Save the remaining information in the pool control block. */
    pool_ptr->ace_os_block_pool_available = blocks;
    pool_ptr->ace_os_block_pool_total = blocks;

    /* Quickly check to make sure at least one block is in the pool */
    if (blocks != 0)
    {
        /* Backup to the last block in the pool. */
        block_ptr = ACE_OS_UCHAR_POINTER_SUB(block_ptr, (block_size + sizeof(ALIGN_TYPE)));

        /* Set the last block's forward pointer to NULL. */
        block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(block_ptr);
        *block_link_ptr = NULL;

        /* Setup the starting pool address. */
        pool_ptr->ace_os_block_pool_available_list = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);

        /* Disable interrupts to place the block pool on the created list.  */
        ACE_OS_DISABLE

        /* Setup the block pool ID to make it valid. */
        // pool_ptr->ace_os_block_pool_id =

        /* Place the block pool on the list of created block pools. First,
            check for an empty list. */
        if (ace_os_block_pool_created_count == ACE_OS_EMPTY)
        {
            ace_os_block_pool_created_ptr                = pool_ptr;
            pool_ptr->ace_os_block_pool_created_next     = pool_ptr;
            pool_ptr->ace_os_block_pool_created_previous = pool_ptr;
        }
        else
        {
            /* This list is not empty, add to the end of the list. */
            next_pool = ace_os_block_pool_created_ptr;
            prev_pool = ace_os_block_pool_created_ptr->ace_os_block_pool_created_previous;

            /* Place the new block pool in the list. */
            next_pool->ace_os_block_pool_created_previous = pool_ptr;
            prev_pool->ace_os_block_pool_created_next = pool_ptr;

            /* Setup this block pool's created links. */
            pool_ptr->ace_os_block_pool_created_next = next_pool;
            pool_ptr->ace_os_block_pool_created_previous = prev_pool;
        }

        /* Increment the created count. */
        ace_os_block_pool_created_count ++;

        /* Restore interrupt. */
        ACE_OS_RESTORE
        
        status = ACE_OS_SUCCESS;
    }
    else
    {
        status = ACE_OS_SIZE_ERROR;
    }

    return status;
}

UINT ace_os_block_pool_delete()
{

}

UINT ace_os_block_pool_info_get()
{

}

VOID ace_os_block_pool_initialize(VOID)
{

}

UINT ace_os_block_pool_performance_info_get()
{

}

UINT ace_os_block_pool_performance_system_info_get()
{

}

UINT ace_block_pool_prioritize()
{

}

UINT ace_block_release(VOID *block_ptr)
{
    ACE_OS_INTERRUPT_SAVE_AREA

    ACE_OS_BLOCK_POOL   *pool_ptr;
    ACE_OS_THREAD       *thread_ptr;
    UCHAR               *work_ptr;
    UCHAR               *return_block_ptr;
    UCHAR               **next_block_ptr;

    /* Disable interrupts to put this block back in the pool.  */
    ACE_OS_DISABLE

    /* Pickup the pool pointer which is just previous to the starting
       address of the block that the caller sees. */
    work_ptr =        ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(block_ptr);
    work_ptr =        ACE_OS_UCHAR_POINTER_SUB(work_ptr, (sizeof(UCHAR *)));
    next_block_ptr =  ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(work_ptr);
    pool_ptr =        ACE_OS_UCHAR_TO_BLOCK_POOL_POINTER_CONVERT((*next_block_ptr));


    /* Determine if there are any threads suspended on the block pool.  */
    thread_ptr =  pool_ptr -> ace_os_block_pool_suspension_list;
    if (thread_ptr != ACE_OS_NULL)
    {

    }
    else
    {
        /* No thread is suspended for a memory block.  */

        /* Put the block back in the available list.  */
        *next_block_ptr =  pool_ptr->ace_os_block_pool_available_list;

        /* Adjust the head pointer.  */
        pool_ptr->ace_os_block_pool_available_list =  work_ptr;

        /* Increment the count of available blocks.  */
        pool_ptr->ace_os_block_pool_available++;

        /* Restore interrupts.  */
        ACE_OS_RESTORE
    }

    /* Return successful completion status.  */
    return(ACE_OS_SUCCESS);
}

/***************************************************************************************************************
**                                                End of file
***************************************************************************************************************/

