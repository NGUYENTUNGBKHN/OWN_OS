/*
****************************************************************************************************************
 * @file       ace_os_api.h
 * @brief      
 * @date       2026/08/09
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
#ifndef _ACE_OS_API_H_
#define _ACE_OS_API_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */
#include "ace_os_port.h"


typedef struct ACE_OS_THREAD_STRUCT
{
    ULONG       ace_os_thread_id;         /* Control block ID */
      

}ACE_OS_THREAD;

#ifdef __cplusplus
}
#endif
#endif
