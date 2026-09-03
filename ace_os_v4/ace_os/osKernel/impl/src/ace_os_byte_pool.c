/*
****************************************************************************************************************
 * @file       ace_os_byte_pool.c
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

/* Define the variable that holds the number of created byte pools. */
ULONG               ace_os_byte_pool_created_count;

/* Define the head pointer of the created byte pool list.  */
ACE_OS_BYTE_POOL    *ace_os_byte_pool_created_ptr;

/***************************************************************************************************************
**                                         INTERNAL FUNCTION PROTOTYPES
***************************************************************************************************************/


/***************************************************************************************************************
**                                             FUNCTION DEFINITIONS
***************************************************************************************************************/

UINT ace_os_byte_allocate(ACE_OS_BYTE_POOL *pool_ptr, VOID **memory_ptr, ULONG memory_size, ULONG wait_option)
{
    ACE_OS_INTERRUPT_SAVE_AREA

    UINT status;
    UINT finish;
    ACE_OS_BYTE_POOL *work_ptr;

    ACE_OS_THREAD *thread_ptr;

    /* Round the memory sizer up to the next size that is ewnely divisible by
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
            finish = ACE_OS_TRUE;
        }
        else
        {
            /* No block was found, does this thread still own the pool. */
            if (pool_ptr->ace_os_byte_pool_owner == thread_ptr)
            {
                /*  */
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

VOID ace_os_byte_pool_clenup()
{

}

UINT ace_os_byte_pool_create(ACE_OS_BYTE_POOL *pool_ptr, CHAR *name_ptr, VOID *pool_start, ULONG pool_size)
{
    ACE_OS_INTERRUPT_SAVE_AREA

    ACE_OS_BYTE_POOL *next_pool;
    ACE_OS_BYTE_POOL *prev_pool;

    UCHAR       *block_ptr;
    UCHAR       *temp_ptr;
    UCHAR       **block_indirect_ptr;
    ALIGN_TYPE  *free_ptr;

    /* Initialize the byte pool control block to all zeros. */
    ACE_OS_MEMSET(pool_ptr, 0, sizeof(ACE_OS_BYTE_POOL));

    /* Round the pool size down to something that is evenly divisible by
        an ULONG */
    pool_size = (pool_size/(sizeof(ALIGN_TYPE)))*(sizeof(ALIGN_TYPE));

    /* Setup the basic byte pool fields. */
    pool_ptr->ace_os_byte_pool_name = name_ptr;

    /* Save the start and size of the pool. */
    pool_ptr->ace_os_byte_pool_start = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);
    pool_ptr->ace_os_byte_pool_size = pool_size;

    /* Setup memory list to the beginning as well as the search pointer.  */
    pool_ptr->ace_os_byte_pool_list =    ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);
    pool_ptr->ace_os_byte_pool_search =  ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);

    /* Initially, the pool will have two blocks. One large block at the 
        beginning that is available and a small allocated block at the end
        of the pool that is there just for the algorithm. Be sure to count
        the available block's header in the available bytes count. */
    pool_ptr->ace_os_byte_pool_available = pool_size - ((sizeof(VOID * )) + (sizeof(ALIGN_TYPE)));
    pool_ptr->ace_os_byte_pool_fragments = ((UINT) 2);

    /* Each block contains a "next" pointer that points to the next block in the pool followed by a ALIGN_TYPE
        field that contains either the constant ACE_BYTE_BLOCK_FREE (if the block is free) or a pointer to the
        owning pool (if the block is allocated). */
    
    /* Calculate the end of the pool's memory area. */
    block_ptr = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);
    block_ptr = ACE_OS_UCHAR_POINTER_ADD(block_ptr, pool_size);

    /* Backup the end of the pool pointer and build the pre-allocated block. */
    block_ptr = ACE_OS_UCHAR_POINTER_SUB(block_ptr, (sizeof(ALIGN_TYPE)));

    /* Cast the pool pointer into a ULONG. */
    temp_ptr             = ACE_OS_BYTE_POOL_TO_UCHAR_POINTER_CONVERT(pool_ptr);
    block_indirect_ptr   = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(block_ptr);
    *block_indirect_ptr  = temp_ptr;

    block_ptr            = ACE_OS_UCHAR_POINTER_SUB(block_ptr, (sizeof(UCHAR*)));
    block_indirect_ptr   = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(block_ptr);
    *block_indirect_ptr  = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);

    /* Now setup the large available block in the pool. */
    temp_ptr             = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);
    block_indirect_ptr   = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(temp_ptr);
    *block_indirect_ptr  = block_ptr;
    block_ptr            = ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(pool_start);
    block_ptr            = ACE_OS_UCHAR_POINTER_ADD(block_ptr, (sizeof(UCHAR*)));
    free_ptr             = ACE_OS_UCHAR_TO_ALIGN_TYPE_POINTER_CONVERT(block_ptr);
    *free_ptr            = ACE_OS_BYTE_BLOCK_FREE;

    /* Clear the owner id. */
    pool_ptr->ace_os_byte_pool_owner = ACE_OS_NULL;

    /* Disable interrupts to place the byte pool on the created list. */
    ACE_OS_DISABLE

    /* Setup the byte pool ID to make it valid. */
    // pool_ptr->ace_os_byte_pool_id = 

    /* Place the byte pool on the list of created byte pools. Frist,
        check for an empty list. */
    if (ace_os_byte_pool_created_count == ACE_OS_EMPTY)
    {
        /* The created byte pool list is empty. Add byte pool to empty list. */
        ace_os_byte_pool_created_ptr             = pool_ptr;
        pool_ptr->ace_os_byte_block_created_next = pool_ptr;
        pool_ptr->ace_os_byte_block_created_prev = pool_ptr;
    }
    else
    {
        /* This list is not NULL, add to the end of the list. */
        next_pool = ace_os_byte_pool_created_ptr;
        prev_pool = next_pool->ace_os_byte_block_created_prev;

        /* Place the new byte pool in the list. */
        next_pool->ace_os_byte_block_created_prev = pool_ptr;
        prev_pool->ace_os_byte_block_created_next = pool_ptr;

        /* Setup this byte pool's created links. */
        pool_ptr->ace_os_byte_block_created_prev = prev_pool;
        pool_ptr->ace_os_byte_block_created_next = next_pool;
    }

    /* Increment the number of created byte pools. */
    ace_os_byte_pool_created_count ++;
    
    ACE_OS_RESTORE

    return ACE_OS_SUCCESS;

}

UINT ace_os_byte_pool_delete()
{

}

UINT ace_os_byte_pool_info_get()
{

}

VOID ace_os_byte_pool_initialize(VOID)
{

}

UINT ace_os_byte_pool_performance_info_get()
{

}

UINT ace_os_byte_pool_performance_system_info_get()
{

}

UINT ace_os_byte_pool_prioritize()
{

}

UCHAR *ace_os_byte_pool_search(ACE_OS_BYTE_POOL *pool_ptr, ULONG memory_size)
{
    ACE_OS_INTERRUPT_SAVE_AREA

    UCHAR           *current_ptr;
    ULONG           total_theoretical_available;
    UCHAR           **this_block_link_ptr;
    ULONG           available_bytes;
    UINT            examine_blocks;
    UCHAR           *work_ptr;
    UCHAR           *free_ptr;
    UCHAR           *next_ptr;
    UCHAR           *next_block_link_ptr;
    UINT            first_free_block_found =  ACE_OS_FALSE;
    ACE_OS_THREAD   *thread_ptr;

    /* Disable interrupt. */
    ACE_OS_DISABLE

    /* First, determine if there are enough bytes in the pool. */
    /* Theoretical bytes available = free bytes + ((fragments-2) * overhead of each block) */
    total_theoretical_available = pool_ptr->ace_os_byte_pool_available + 
                            ((pool_ptr->ace_os_byte_pool_fragments -2) * ((sizeof(UCHAR*)) + (sizeof(ALIGN_TYPE))));
    if (memory_size >= total_theoretical_available)
    {
        /* Restore interrupt. */
        ACE_OS_RESTORE

        /* Not enough memory, return a NULL pointer. */
        current_ptr = ACE_OS_NULL;
    }
    else
    {
        /* Pickup thread pointer. */
        ACE_OS_THREAD_GET_CURRENT(thread_ptr);

        /* Setup ownership of the byte pool. */
        pool_ptr->ace_os_byte_pool_owner = thread_ptr;

        /* Walk through the memory pool in search for a large enough block. */
        current_ptr = pool_ptr->ace_os_byte_pool_search;
        examine_blocks = pool_ptr->ace_os_byte_pool_fragments + ((UINT) 1);
        available_bytes = ((ULONG) 0);
        do
        {
            /* Check to see if this block is free. */
            work_ptr = ACE_OS_UCHAR_POINTER_ADD(current_ptr, (sizeof(UCHAR*)));
            free_ptr = ACE_OS_UCHAR_TO_ALIGN_TYPE_POINTER_CONVERT(work_ptr);
            if ((*free_ptr) == ACE_OS_BYTE_BLOCK_FREE)
            {
                /* Determine if this is the first free block. */
                if (first_free_block_found == ACE_OS_FALSE)
                {
                    /* This is the first free block. */
                    pool_ptr->ace_os_byte_pool_search = current_ptr;

                    /* Set the flag to indicate we have found the first free
                        block. */
                    first_free_block_found = ACE_OS_TRUE;
                }

                /* Block is free, see if it is large enough. */

                /* Pickup the next block's pointer */
                this_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(current_ptr);
                next_ptr            = *this_block_link_ptr;

                /* Calculate the number of bytes available in this block. */
                available_bytes = ACE_OS_UCHAR_POINTER_DIF(next_ptr, current_ptr);
                available_bytes = available_bytes - ((sizeof(UCHAR*)) + (sizeof(ALIGN_TYPE)));

                /* If this is large enough, we are done because our first-fit algorithm
                    has been satisfied! */
                if (available_bytes >= memory_size)
                {
                    /* Get out of the search loop! */
                    break;
                }
                else
                {
                    /* Clear the available bytes variable. */
                    available_bytes = ((ULONG) 0);

                    /* Not enough memory, check to see if the neighbor is
                        free and can be merged. */
                    work_ptr = ACE_OS_UCHAR_POINTER_ADD(next_ptr, (sizeof(UCHAR*)));
                    free_ptr = ACE_OS_UCHAR_TO_ALIGN_TYPE_POINTER_CONVERT(work_ptr);
                    if ((*free_ptr) == ACE_OS_BYTE_BLOCK_FREE)
                    {
                        /* Yse, neighbor block can be merged! This is quickly acccomplished
                            by updating the current block with the nexCalt blocks pointer. */
                        next_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(next_ptr);
                        *this_block_link_ptr = *next_block_link_ptr;

                        /* Reduce the fragment total. We don't need to increase the bytes
                            available because all free headers are also included in the available 
                            count. */
                        pool_ptr->ace_os_byte_pool_fragments;

                        /* See if the search pointer is affected. */
                        if (pool_ptr->ace_os_byte_pool_search)
                        {
                            /* Update the search pointer. */
                            pool_ptr->ace_os_byte_pool_search = current_ptr;
                        }
                    }
                    else
                    {
                        /* Neighbor is not free so we can skip over it! */
                        next_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(next_ptr);
                        current_ptr = *next_block_link_ptr;

                        /* Decrement the examined block count to account for this one. */
                        if (examine_blocks != ((UINT) 0))
                        {
                            examine_blocks--;
                        }
                    }
                }
            }
            else
            {
                /* Block is not free, move to next block. */
                this_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(current_ptr);
                current_ptr = *this_block_link_ptr;

            }

            /*  Another block has been searched ... decrement counter. */
            if (examine_blocks != ((UINT) 0))
            {
                examine_blocks --;
            }

            /* Restore interrupt temporarily. */
            ACE_OS_RESTORE

            /* Disable interrupt. */
            ACE_OS_DISABLE

            /* Determine if anything has changed in terms of pool ownership. */
            if (pool_ptr->ace_os_byte_pool_owner != thread_ptr)
            {
                /* Pool changed onwership in the brief period interrup were
                    enabled. Reset the search. */
                current_ptr = pool_ptr->ace_os_byte_pool_search;
                examine_blocks = pool_ptr->ace_os_byte_pool_fragments + ((UINT) 1);

                /* Setup our ownership again. */
                pool_ptr->ace_os_byte_pool_owner = thread_ptr;
            }

        } while (examine_blocks != ((ULONG) 0));
        
        /* Determine if a block was found. if so, determine if it needs to be 
            split. */
        if (available_bytes != ((ULONG) 0))
        {
            /* Determine if we need to split this block. */
            if ((available_bytes - memory_size) >= ((ULONG) ACE_OS_BYTE_BLOCK_MIN))
            {
                /* Split the block. */
                next_ptr = ACE_OS_UCHAR_POINTER_ADD(current_ptr, (memory_size + ((sizeof(UCHAR*)) * (sizeof(ALIGN_TYPE)))));

                /* Setup the new free block. */
                next_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(next_ptr);
                this_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(current_ptr);
                *next_block_link_ptr = *this_block_link_ptr;       

            }

            /* Restore interrupts. */
            ACE_OS_RESTORE

            /* Adjust the pointer for the application. */
            current_ptr = ACE_OS_UCHAR_POINTER_ADD(current_ptr, (((sizeof(UCHAR*)) + (sizeof(ALIGN_TYPE)))));
        }
        else
        {
            /* Restore interrupt. */
            ACE_OS_RESTORE

            /* Set current pointer to NULL to indicate nothing was found. */
            current_ptr = ACE_OS_NULL;
        }

    }

    /* Return the search pointer. */
    return (current_ptr);
}

UINT ace_byte_release()
{
    
}


/***************************************************************************************************************
**                                                End of file
***************************************************************************************************************/

