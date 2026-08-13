/*
****************************************************************************************************************
 * @file       ace_os_port.h
 * @brief      
 * @date       2026/08/09
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
****************************************************************************************************************
*/
#ifndef _ACE_OS_PORT_H_
#define _ACE_OS_PORT_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */
#include "stdlib.h"
#include "string.h"



/* Define ThreadX basic types for this port. */
#define VOID                        void
typedef char                        CHAR;
typedef unsigned char               UCHAR;
typedef int                         INT;
typedef unsigned int                UINT;
typedef long                        LONG;
typedef unsigned long               ULONG;
typedef short                       SHORT;
typedef unsigned short              USHORT;

/* Define the interrupt disable/restore macros for each compiler. */

#if defined(__GNUC__) || defined(__ICCARM__)



#define ACE_OS_INTERRUPT_SAVE_AREA          UINT interrupt_save;
#define ACE_OS_DISABLE                      interrupt_save = __disable_interrupt();
#define ACE_OS_RESTORE                      __restore_interrupt(interrupt_save);
#else

#endif

#ifdef __cplusplus
}
#endif
#endif
