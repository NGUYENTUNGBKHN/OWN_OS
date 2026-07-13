/**
 * @file       ace_os_type.h
 * @brief      
 * @date       2026/07/07
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
*/
#ifndef _ACE_OS_TYPE_H_
#define _ACE_OS_TYPE_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* CODE */

/*
************************************************************************************************************************
*                                                 INCLUDE HEADER FILES
************************************************************************************************************************
*/
                                                       /*       Description                                    # Bits */
                                                       /*                                               <recommended> */
                                                       /* ----------------------------------------------------------- */

typedef   CPU_INT16U      ACE_OS_CPU_USAGE;                /* CPU Usage 0..10000                                  <16>/32 */

typedef   CPU_INT32U      ACE_OS_CTR;                      /* Counter,                                                 32 */

typedef   CPU_INT32U      ACE_OS_CTX_SW_CTR;               /* Counter of context switches,                             32 */

typedef   CPU_INT32U      ACE_OS_CYCLES;                   /* CPU clock cycles,                                   <32>/64 */

typedef   CPU_INT32U      ACE_OS_FLAGS;                    /* Event flags,                                      8/16/<32> */

typedef   CPU_INT32U      ACE_OS_IDLE_CTR;                 /* Holds the number of times the idle task runs,       <32>/64 */

typedef   CPU_INT16U      ACE_OS_MEM_QTY;                  /* Number of memory blocks,                            <16>/32 */
typedef   CPU_INT16U      ACE_OS_MEM_SIZE;                 /* Size in bytes of a memory block,                    <16>/32 */

typedef   CPU_INT16U      ACE_OS_MSG_QTY;                  /* Number of OS_MSGs in the msg pool,                  <16>/32 */
typedef   CPU_INT16U      ACE_OS_MSG_SIZE;                 /* Size of messages in number of bytes,                <16>/32 */

typedef   CPU_INT08U      ACE_OS_NESTING_CTR;              /* Interrupt and scheduler nesting,                  <8>/16/32 */

typedef   CPU_INT16U      ACE_OS_OBJ_QTY;                  /* Number of kernel objects counter,                   <16>/32 */
typedef   CPU_INT32U      ACE_OS_OBJ_TYPE;                 /* Special flag to determine object type,                   32 */

typedef   CPU_INT16U      ACE_OS_OPT;                      /* Holds function options,                             <16>/32 */

typedef   CPU_INT32U      ACE_OS_MON_RES;                  /* Monitor result flags,                                       */

typedef   CPU_INT08U      ACE_OS_PRIO;                     /* Priority of a task,                               <8>/16/32 */

typedef   CPU_INT16U      ACE_OS_QTY;                      /* Quantity                                            <16>/32 */

typedef   CPU_INT32U      ACE_OS_RATE_HZ;                  /* Rate in Hertz                                            32 */

#if (CPU_CFG_ADDR_SIZE == CPU_WORD_SIZE_64)                 /* Task register                                  8/16/<32/64> */
typedef   CPU_INT64U      ACE_OS_REG;
#else
typedef   CPU_INT32U      ACE_OS_REG;
#endif
typedef   CPU_INT08U      ACE_OS_REG_ID;                   /* Index to task register                            <8>/16/32 */

typedef   CPU_INT32U      ACE_OS_SEM_CTR;                  /* Semaphore value                                     16/<32> */

typedef   CPU_INT08U      ACE_OS_STATE;                    /* State variable                                    <8>/16/32 */

typedef   CPU_INT08U      ACE_OS_STATUS;                   /* Status                                            <8>/16/32 */

typedef   CPU_INT32U      ACE_OS_TICK;                     /* Clock tick counter                                  <32>/64 */

#ifdef __cplusplus
}
#endif
#endif
