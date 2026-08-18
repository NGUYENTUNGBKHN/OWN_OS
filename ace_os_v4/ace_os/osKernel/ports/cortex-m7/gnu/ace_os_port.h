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

/* Define Align type. --> 32-bit */
#define ALIGN_TYPE                  ULONG


/* Define the interrupt disable/restore macros for each compiler. */

#if defined(__GNUC__) || defined(__ICCARM__)

__attribute__( ( always_inline ) ) static inline unsigned int __get_interrupt_posture()
{
    unsigned int posture;
#ifdef ACE_OS_PORT_USE_BASEPRI
    __asm__ volatile ("MRS  %0, BASEPRI ": "=r" (posture));
#else   
    __asm__ volatile ("MRS  %0, PRIMASK ": "=r" (posture));
#endif 
    return posture;
}

#ifdef ACE_OS_PORT_USE_BASEPRI
__attribute__( ( always_inline ) ) static inline void __set_basepri_value(unsigned int basepri_value)
{
    __asm__ volatile ("MSR  BASEPRI,%0 ": : "r" (basepri_value) : "memory");
}
#else

__attribute__( ( always_inline ) ) static inline void __enable_interrupts(void)
{
    __asm__ volatile ("CPSIE  i": : : "memory");
}
#endif 

__attribute__( ( always_inline ) ) static inline void __restore_interrupt(unsigned int int_posture)
{
#ifdef ACE_OS_PORT_USE_BASEPRI
    __set_basepri_value(int_posture);
    __asm__ volatile ("" : : : "memory");
#else
    __asm__ volatile ("MRS  PRIMASK,%0": : "r" (int_posture): "memory");
#endif 
}

__attribute__( ( always_inline ) ) static inline unsigned int __disable_interrupt(void)
{
unsigned int int_posture;

    int_posture = __get_interrupt_posture();

#ifdef TX_PORT_USE_BASEPRI
    __set_basepri_value(TX_PORT_BASEPRI);
#else
    __asm__ volatile ("CPSID i" : : : "memory");
#endif
    return(int_posture);
}


#define ACE_OS_INTERRUPT_SAVE_AREA          UINT interrupt_save;
#define ACE_OS_DISABLE                      interrupt_save = __disable_interrupt();
#define ACE_OS_RESTORE                      __restore_interrupt(interrupt_save);
#else

#endif

#ifdef __cplusplus
}
#endif
#endif
