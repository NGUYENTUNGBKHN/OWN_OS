/*
****************************************************************************************************************
 * @file       ace_os_user.h
 * @brief      
 * @date       2026/09/05
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/

#ifndef _ACE_OS_USER_H_
#define _ACE_OS_USER_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */

/* Determine if preemption-threshold should be disabled. By default, preemption-threshold is
   enabled. If the application does not use preemption-threshold, it may be disabled to reduce
   code size and improve performance.  */

#undef ACE_OS_DISABLE_PREEMPTION_THRESHOLD  


#ifdef __cplusplus
}
#endif
#endif