/*
****************************************************************************************************************
 * @file       ace_os_byte_pool_search.c
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


UCHAR *ace_os_byte_pool_search(ACE_OS_BYTE_POOL *pool_ptr, ULONG memory_size)
{
    ACE_OS_INTERRUPT_SAVE_AREA

    UCHAR           *current_ptr;
    ULONG           total_theoretical_available;
    UCHAR           **this_block_link_ptr;
    ULONG           available_bytes;
    UINT            examine_blocks;
    UCHAR           *work_ptr;
    ALIGN_TYPE      *free_ptr;
    UCHAR           *next_ptr;
    UCHAR           **next_block_link_ptr;
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
                        pool_ptr->ace_os_byte_pool_fragments--;

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
                next_ptr = ACE_OS_UCHAR_POINTER_ADD(current_ptr, (memory_size + ((sizeof(UCHAR*)) + (sizeof(ALIGN_TYPE)))));

                /* Setup the new free block. */
                next_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(next_ptr);
                this_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(current_ptr);
                *next_block_link_ptr = *this_block_link_ptr;       
                work_ptr            = ACE_OS_UCHAR_POINTER_ADD(next_ptr, (sizeof(UCHAR *)));
                free_ptr            = ACE_OS_UCHAR_TO_ALIGN_TYPE_POINTER_CONVERT(work_ptr);
                *free_ptr           = ACE_OS_BYTE_BLOCK_FREE;

                /* Increase the total fragment counter. */
                pool_ptr->ace_os_byte_pool_fragments++;

                /* Update the current pointer to point at the newly created block. */
                *this_block_link_ptr = next_ptr;

                /* Set available equal to memory size for subsequent calcualation. */
                available_bytes = memory_size;


            }

            /* In any case, mark the current block as allocated. */
            work_ptr            = ACE_OS_UCHAR_POINTER_ADD(current_ptr, (sizeof(UCHAR *)));
            this_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(work_ptr);
            *this_block_link_ptr = ACE_OS_BYTE_POOL_TO_UCHAR_POINTER_CONVERT(pool_ptr);

            /* Reduce the number of available bytes in the pool. */
            pool_ptr->ace_os_byte_pool_available = (pool_ptr->ace_os_byte_pool_available - available_bytes) 
                                                - ((sizeof(UCHAR *)) + (sizeof(ALIGN_TYPE)));


            /* Determine if the search pointer needs to be updated. This is only done
                if the search pointer matches the block to be returned. */
            if (current_ptr == pool_ptr->ace_os_byte_pool_search)
            {
                /* Yse, update the search pointer to the next block. */
                this_block_link_ptr = ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(current_ptr);
                pool_ptr->ace_os_byte_pool_search = * this_block_link_ptr;
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






/************************************************ End of file *************************************************/

