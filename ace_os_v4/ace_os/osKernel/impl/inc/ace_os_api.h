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


/*
************************************************************************************************************************
************************************************************************************************************************
*                                       A C E - O S   D E F I N I T I O N
************************************************************************************************************************
************************************************************************************************************************
*/
#define ACE_OS_STACK_FILL                   ((ULONG)  0xEFEFEFEFUL)

/* Thread execution state values.  */

#define ACE_OS_READY                        ((UINT) 0)
#define ACE_OS_COMPLETED                    ((UINT) 1)
#define ACE_OS_TERMINATED                   ((UINT) 2)
#define ACE_OS_SUSPENDED                    ((UINT) 3)
#define ACE_OS_SLEEP                        ((UINT) 4)
#define ACE_OS_QUEUE_SUSP                   ((UINT) 5)
#define ACE_OS_SEMAPHORE_SUSP               ((UINT) 6)
#define ACE_OS_EVENT_FLAG                   ((UINT) 7)
#define ACE_OS_BLOCK_MEMORY                 ((UINT) 8)
#define ACE_OS_BYTE_MEMORY                  ((UINT) 9)
#define ACE_OS_IO_DRIVER                    ((UINT) 10)
#define ACE_OS_FILE                         ((UINT) 11)
#define ACE_OS_TCP_IP                       ((UINT) 12)
#define ACE_OS_MUTEX_SUSP                   ((UINT) 13)
#define ACE_OS_PRIORITY_CHANGE              ((UINT) 14)

/*
************************************************************************************************************************
************************************************************************************************************************
*                           A C E - O S   S T R U C T U R E   D E F I N I T I O N
************************************************************************************************************************
************************************************************************************************************************
*/

typedef struct ACE_OS_THREAD_STRUCT
{
    ULONG       ace_os_thread_id;                       /* Control block ID */
    
    VOID        *ace_os_thread_stack_ptr;               /* ACE OS's stack pointer */
    VOID        *ace_os_thread_stack_start;             /* ACE_OS's stack start address */
    VOID        *ace_os_thread_stack_end;               /* ACE_OS's stack end address */
    ULONG       ace_os_thread_stack_size;

    struct ACE_OS_THREAD_STRUCT
                *ace_os_thread_ready_next,
                *ace_os_thread_ready_previous;
    
    CHAR        *ace_os_thread_name;
    UINT        ace_os_thread_priority;
    UINT        ace_os_thread_state;

    VOID        (*ace_os_thread_entry)(ULONG id);
    ULONG       ace_os_thread_entry_params;

}ACE_OS_THREAD;

/*
************************************************************************************************************************
************************************************************************************************************************
*                           A C E - O S   
************************************************************************************************************************
************************************************************************************************************************
*/

#define ACE_OS_MEMSET(a, b, c)                          memset((a), (b), (c))

#define ACE_OS_UCHAR_POINTER_ADD(a, b)                  (((UCHAR *)(a)) + ((UINT)(b)))

/* Convert void between char */
#define ACE_OS_VOID_TO_CHAR_POINTER_CONVERT(tmp)        ((UCHAR *)((VOID *)(tmp)))
#define ACE_OS_CHAR_TO_VOID_POINTER_CONVERT(tmp)        ((VOID *)(tmp))


#ifdef __cplusplus
}
#endif
#endif
