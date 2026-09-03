/*
****************************************************************************************************************
 * @file       ace_os_byte_pool.h
 * @brief      
 * @date       2026/08/09
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
#ifndef _ACE_OS_BYTE_POOL_H_
#define _ACE_OS_BYTE_POOL_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */

/* Dfine byte memory control specific data definitions. */

#define ACE_OS_BYTE_POOL_ID                         ((ULONG) 0x42595445)

#ifndef ACE_OS_BYTE_BLOCK_FREE
#define ACE_OS_BYTE_BLOCK_FREE                      ((ULONG) 0xFFFFEEEEUL)
#endif

#ifndef ACE_OS_BYTE_BLOCK_MIN
#define ACE_OS_BYTE_BLOCK_MIN                       ((ULONG) 20)
#endif

#ifndef ACE_OS_BYTE_POOL_MIN
#define ACE_OS_BYTE_POOL_MIN                        ((ULONG) 100)
#endif 

/* No in-line initialization is supported, use standard function call. */
VOID ace_os_byte_pool_initialize(VOID);


/* Define internal byte memory pool management function prototypes. */

UCHAR   *ace_os_byte_pool_search(ACE_OS_BYTE_POOL *pool_ptr, ULONG memory_size);
VOID    ace_os_byte_pool_cleanup(ACE_OS_THREAD *thread_ptr, ULONG suspension_sequence);

#define BYTE_POOL_DECLARE extern


/* Define the head pointer of the created byte pool list. */

BYTE_POOL_DECLARE ACE_OS_BYTE_POOL *ace_os_byte_pool_created_ptr;

/* Define the variable that holds the number of created byte pools. */

BYTE_POOL_DECLARE ULONG ace_os_byte_pool_created_count;



#ifdef __cplusplus
}
#endif
#endif
