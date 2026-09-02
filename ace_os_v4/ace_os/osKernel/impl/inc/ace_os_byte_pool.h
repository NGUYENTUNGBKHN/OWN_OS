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

#ifndef ACE_OS_BYTE_BLOCK_FREE
#define ACE_OS_BYTE_BLOCK_FREE                      ((ULONG) 0xFFFFEEEEUL)
#endif

#ifndef ACE_OS_BYTE_BLOCK_MIN
#define ACE_OS_BYTE_BLOCK_MIN                       ((ULONG) 20)
#endif

#ifndef ACE_OS_BYTE_POOL_MIN
#define ACE_OS_BYTE_POOL_MIN                        ((ULONG) 100)
#endif 

#ifdef __cplusplus
}
#endif
#endif
