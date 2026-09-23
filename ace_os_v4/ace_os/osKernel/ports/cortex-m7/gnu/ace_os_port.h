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


/* Define the priority levels for ThreadX. Legal values range
    from 32 to 1024 and MUST be evenly divisiable by 32. */
#ifndef ACE_OS_MAX_PRIORITIES
#define ACE_OS_MAX_PRIORITIES   32
#endif 

#ifndef ACE_OS_THREAD_GET_SYSTEM_STATE

#if defined(__GNUC__)
__attribute__( ( always_inline ) ) static inline unsigned int __get_ipsr_value(void)
{
unsigned int  ipsr_value;
    __asm__ volatile (" MRS  %0,IPSR ": "=r" (ipsr_value) );
    return(ipsr_value);
}

#define ACE_OS_THREAD_GET_SYSTEM_STATE()           (ace_os_thread_system_state | __get_ipsr_value())
#endif // __GNUC__
#endif // ACE_OS_THREAD_GET_SYSTEM_STATE

/* Define the check for whether or not to call the _tx_thread_system_return function.  A non-zero value
   indicates that _tx_thread_system_return should not be called. This overrides the definition in tx_thread.h
   for Cortex-M since so we don't waste time checking the _tx_thread_system_state variable that is always
   zero after initialization for Cortex-M ports. */

#ifndef ACE_OS_THREAD_SYSTEM_RETURN_CHECK
#define ACE_OS_THREAD_SYSTEM_RETURN_CHECK(c)        (c) = ((ULONG) ace_os_thread_preempt_disable);
#endif 

/* Define the macro to ensure ace_os_thread_preempt_disable is set early in initialization in order to 
    prevent early scheduling on Cortex-M parts. */

#define ACE_OS_PORT_SPECIFIC_POST_INITIALIZATION     ace_os_thread_preempt_disable++;

#ifndef ACE_OS_DISABLE_INLINE
/* Define the ACE_OS_LOWEST_SET_BIT_CALCULATE macro for each compiler. */
#ifdef __ICCAM__            /* IAR complier */
#elif defined(__CC_ARM)     /* AC5 compiler */
#elif defined(__GNUC__)     /* GCC and AC6 compiler */

#define ACE_OS_LOWEST_SET_BIT_CALCULATE(m,b)    __asm__ volatile (" RBIT %0,%1": "=r" (m): "r" (m) ); \
                                                __asm__ volatile (" CLZ  %0,%1": "=r" (b): "r" (m));

#endif // compiler

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
    __asm__ volatile ("MSR  PRIMASK,%0": : "r" (int_posture): "memory");
#endif 
}

__attribute__( ( always_inline ) ) static inline unsigned int __disable_interrupt(void)
{
unsigned int int_posture;

    int_posture = __get_interrupt_posture();

#ifdef ACE_OS_PORT_USE_BASEPRI
    __set_basepri_value(ACE_OS_PORT_BASEPRI);
#else
    __asm__ volatile ("CPSID i" : : : "memory");
#endif
    return(int_posture);
}

__attribute__( ( always_inline ) ) static inline void ace_os_thread_system_return_inline(void)
{
unsigned int interrupt_save;

    /* Set PendSV to invoke ThreadX scheduler. */
    *((volatile ULONG *) 0xE000ED04) = ((ULONG) 0x10000000);
    __asm__ volatile ("dsb 0xF \n isb 0xF" : : : "memory");
    if (__get_ipsr_value() == 0)
    {
        interrupt_save = __get_interrupt_posture();
#ifdef ACE_OS_PORT_USE_BASEPRI
        __set_basepri_value(0);
#else
        __enable_interrupts();
#endif
        __restore_interrupt(interrupt_save);
        __asm__ volatile ("isb 0xF " : : : "memory");
    }
}


#define ACE_OS_INTERRUPT_SAVE_AREA          UINT interrupt_save;
#define ACE_OS_DISABLE                      interrupt_save = __disable_interrupt();
#define ACE_OS_RESTORE                      __restore_interrupt(interrupt_save);
#else   // not __GNUC__ || __ICCARM__

#endif // __GNUC__ || __ICCARM__

/* Redefine ace_os_thread_system_return for improved performance */
#define ace_os_thread_system_return     ace_os_thread_system_return_inline

#else   // define ACE_OS_DISABLE_INLINE

#endif  // ACE_OS_DISABLE_INLINE

#ifdef __cplusplus
}
#endif
#endif
