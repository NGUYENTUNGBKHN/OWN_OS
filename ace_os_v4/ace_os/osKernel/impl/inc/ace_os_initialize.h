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

/* Define internal initialization function prototypes.  */

VOID        ace_os_initialize_high_level(VOID);
VOID        ace_os_initialize_kernel_setup(VOID);
VOID        ace_os_initialize_low_level(VOID);

#define INITIALIZE_DECLARE  extern

/* Define the unused memory pointer. The value of the first available
    memory address is placed in this variable in the low-level
    initialization function. The content of this variable is passed
    to the application's system dfinition function. */

INITIALIZE_DECLARE  VOID    *ace_os_initialize_unused_memory;


#ifdef __cplusplus
}
#endif
#endif
