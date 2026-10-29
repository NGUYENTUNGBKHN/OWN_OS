/*
****************************************************************************************************************
 * @file       ace_os_byte_pool_create.c
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
// #include "ace_os_thread.h"
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
 * @brief      
 * @param      pool_ptr    
 * @param      name_ptr    
 * @param      pool_start    
 * @param      pool_size    
 * @return     UINT
****************************************************************************************************************
*/
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
    pool_ptr->ace_os_byte_pool_id = ACE_OS_BYTE_POOL_ID;

    /* Place the byte pool on the list of created byte pools. First,
        check for an empty list. */
    if (ace_os_byte_pool_created_count == ACE_OS_EMPTY)
    {
        /* The created byte pool list is empty. Add byte pool to empty list. */
        ace_os_byte_pool_created_ptr             = pool_ptr;
        pool_ptr->ace_os_byte_pool_created_next = pool_ptr;
        pool_ptr->ace_os_byte_pool_created_prev = pool_ptr;
    }
    else
    {
        /* This list is not NULL, add to the end of the list. */
        next_pool = ace_os_byte_pool_created_ptr;
        prev_pool = next_pool->ace_os_byte_pool_created_prev;

        /* Place the new byte pool in the list. */
        next_pool->ace_os_byte_pool_created_prev = pool_ptr;
        prev_pool->ace_os_byte_pool_created_next = pool_ptr;

        /* Setup this byte pool's created links. */
        pool_ptr->ace_os_byte_pool_created_prev = prev_pool;
        pool_ptr->ace_os_byte_pool_created_next = next_pool;
    }

    /* Increment the number of created byte pools. */
    ace_os_byte_pool_created_count ++;
    
    /* Restore interrupts. */
    ACE_OS_RESTORE

    /* Return ACE_OS_SUCCESS. */
    return ACE_OS_SUCCESS;

}
/************************************************ End of file *************************************************/

