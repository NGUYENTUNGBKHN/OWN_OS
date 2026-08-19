/*
****************************************************************************************************************
 * @file       ace_os_block_pool.h
 * @brief      
 * @date       2026/08/09
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
#ifndef _ACE_OS_BLOCK_POOL_H_
#define _ACE_OS_BLOCK_POOL_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */

#ifdef ACE_OS_BLOCK_POOL_INIT
#define BLOCK_POOL_DECLARE
#else
#define BLOCK_POOL_DECLARE extern
#endif

/* Define the head pointer of the created block pool list.  */

BLOCK_POOL_DECLARE  ACE_OS_BLOCK_POOL *         ace_os_block_pool_created_ptr;


/* Define the variable that holds the number of created block pools. */

BLOCK_POOL_DECLARE  ULONG                   ace_os_block_pool_created_count;

#ifdef __cplusplus
}
#endif
#endif
