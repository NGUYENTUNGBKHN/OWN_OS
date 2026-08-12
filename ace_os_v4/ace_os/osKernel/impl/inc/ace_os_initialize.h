/*
****************************************************************************************************************
 * @file       ace_os_initialize.h
 * @brief      
 * @date       2026/08/09
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
#ifndef _ACE_OS_INITIALIZE_H_
#define _ACE_OS_INITIALIZE_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */

/* Define constants that indicate initialization is in progress.  */

#define ACE_OS_INITIALIZE_IN_PROGRESS               ((ULONG) 0xF0F0F0F0UL)
#define ACE_OS_INITIALIZE_ALMOST_DONE               ((ULONG) 0xF0F0F0F1UL)
#define ACE_OS_INITIALIZE_IS_FINISHED               ((ULONG) 0x00000000UL)

#ifdef __cplusplus
}
#endif
#endif
