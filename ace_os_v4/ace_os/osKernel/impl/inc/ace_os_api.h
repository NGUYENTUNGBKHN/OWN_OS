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

/* API input parameters and general constants.  */
#define ACE_OS_NO_WAIT                      ((ULONG)  0)
#define ACE_OS_EMPTY                        ((ULONG)  0)
#define ACE_OS_AUTO_START                   ((UINT)   1)
#define ACE_OS_NULL                         ((ULONG)  0)
#define ACE_OS_FALSE                        ((UINT)   0)
#define ACE_OS_TRUE                         ((UINT)   1)
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


/* API return values.  */

#define ACE_OS_SUCCESS                      ((UINT) 0x00)
#define ACE_OS_DELETED                      ((UINT) 0x01)
#define ACE_OS_POOL_ERROR                   ((UINT) 0x02)
#define ACE_OS_PTR_ERROR                    ((UINT) 0x03)
#define ACE_OS_WAIT_ERROR                   ((UINT) 0x04)
#define ACE_OS_SIZE_ERROR                   ((UINT) 0x05)
#define ACE_OS_GROUP_ERROR                  ((UINT) 0x06)
#define ACE_OS_NO_EVENTS                    ((UINT) 0x07)
#define ACE_OS_OPTION_ERROR                 ((UINT) 0x08)
#define ACE_OS_QUEUE_ERROR                  ((UINT) 0x09)
#define ACE_OS_QUEUE_EMPTY                  ((UINT) 0x0A)
#define ACE_OS_QUEUE_FULL                   ((UINT) 0x0B)
#define ACE_OS_SEMAPHORE_ERROR              ((UINT) 0x0C)
#define ACE_OS_NO_INSTANCE                  ((UINT) 0x0D)
#define ACE_OS_THREAD_ERROR                 ((UINT) 0x0E)
#define ACE_OS_PRIORITY_ERROR               ((UINT) 0x0F)
#define ACE_OS_NO_MEMORY                    ((UINT) 0x10)
#define ACE_OS_START_ERROR                  ((UINT) 0x10)
#define ACE_OS_DELETE_ERROR                 ((UINT) 0x11)
#define ACE_OS_RESUME_ERROR                 ((UINT) 0x12)
#define ACE_OS_CALLER_ERROR                 ((UINT) 0x13)
#define ACE_OS_SUSPEND_ERROR                ((UINT) 0x14)
#define ACE_OS_TIMER_ERROR                  ((UINT) 0x15)
#define ACE_OS_TICK_ERROR                   ((UINT) 0x16)
#define ACE_OS_ACTIVATE_ERROR               ((UINT) 0x17)
#define ACE_OS_THRESH_ERROR                 ((UINT) 0x18)
#define ACE_OS_SUSPEND_LIFTED               ((UINT) 0x19)
#define ACE_OS_WAIT_ABORTED                 ((UINT) 0x1A)
#define ACE_OS_WAIT_ABORT_ERROR             ((UINT) 0x1B)
#define ACE_OS_MUTEX_ERROR                  ((UINT) 0x1C)
#define ACE_OS_NOT_AVAILABLE                ((UINT) 0x1D)
#define ACE_OS_NOT_OWNED                    ((UINT) 0x1E)
#define ACE_OS_INHERIT_ERROR                ((UINT) 0x1F)
#define ACE_OS_NOT_DONE                     ((UINT) 0x20)
#define ACE_OS_CEILING_EXCEEDED             ((UINT) 0x21)
#define ACE_OS_INVALID_CEILING              ((UINT) 0x22)
#define ACE_OS_FEATURE_NOT_ENABLED          ((UINT) 0xFF)

/*
************************************************************************************************************************
************************************************************************************************************************
*                           A C E - O S   S T R U C T U R E   D E F I N I T I O N
************************************************************************************************************************
************************************************************************************************************************
*/

/* Define the block memory pool structure utilized by the application.  */

typedef struct ACE_OS_BLOCK_POOL_STRUCT
{
    /* Define the block pool ID used for error checking. */
    ULONG           ace_os_block_pool_id;

    /* Define the block pool's name. */
    CHAR            *ace_os_block_pool_name;

    /* Define the number of available memory blocks in the pool. */
    UINT            ace_os_block_pool_available;

    /* Save the initial number of block */
    UINT            ace_os_block_pool_total;

    /* Define the head pointer of the available block pool. */
    UCHAR           *ace_os_block_pool_available_list;

    /* Save the start address of the block pool's memory area. */
    UCHAR           *ace_os_block_pool_start;

    /* Save the block pool's size in bytes */
    ULONG           ace_os_block_pool_size;

    /* Save the individual memory block size - rounded for alignment. */
    UINT            ace_os_block_pool_block_size;

    struct ACE_OS_BLOCK_POOL_STRUCT
                    ace_os_block_pool_suspension_list;
    UINT            ace_os_block_pool_suspension_cnt;

    /* Define the created list next and previous pointer. */
    struct ACE_OS_BLOCK_POOL_STRUCT
                    *ace_os_block_pool_created_next,
                    *ace_os_block_pool_created_previous;

}ACE_OS_BLOCK_POOL;

/* Define the byte memory pool structure utilized by the application. */
typedef struct ACE_OS_BYTE_POOL_STRUCT
{
   /* Defined the byte pool ID used for error checking. */
   ULONG       ace_os_byte_pool_id;

   /* Define the byte pool's name. */
   CHAR        *ace_os_byte_pool_name;

   /* Define the number of available bytes in the pool. */
   ULONG       *ace_os_byte_pool_available;

   /* Define the number of fragments in the pool. */
   UINT        ace_os_byte_pool_fragments;

   /* Define the head pointer of byte pool. */
   UCHAR       *ace_os_byte_pool_list;

   /* Define the search pointer used for initial searching for memory
      in a byte pool. */
   UCHAR       *ace_os_byte_pool_search;

   /* Save the start address of the byte pool's memory area. */
   UCHAR       *ace_os_byte_pool_start;

   /* Save the byte pool's size in bytes. */
   ULONG       ace_os_byte_pool_size;

   /* This is used to mark the owner of the byte memory pool during
      a search. If this value changes during the search, the local search
      pointer must be reset. */
   struct ACE_OS_THREAD_STRUCT
               *ace_os_byte_pool_owner;
   
   /* Define the byte pool suspension list head along with a count of
      how many threads are suspended. */
   struct ACE_OS_BYTE_POOL_STRUCT
               *ace_os_byte_pool_suspension_list;
   UINT        ace_os_byte_pool_suspension_count;

   /* Define the created list next and previous pointer. */
   struct ACE_OS_BLOCK_POOL_STRUCT
               *ace_os_byte_block_created_next,
               *ace_os_byte_block_created_prev;

}ACE_OS_BYTE_POOL;



/* ThreadX thread control block structure follows.  Additional fields
   can be added providing they are added after the information that is
   referenced in the port-specific assembly code.  */

typedef struct ACE_OS_THREAD_STRUCT
{
    ULONG       ace_os_thread_id;                       /* Control block ID                 */
    ULONG       ace_os_thread_run_cnt;                  /* Thread's run counter             */
    VOID        *ace_os_thread_stack_ptr;               /* ACE OS's stack pointer           */
    VOID        *ace_os_thread_stack_start;             /* ACE_OS's stack start address     */
    VOID        *ace_os_thread_stack_end;               /* ACE_OS's stack end address       */
    ULONG       ace_os_thread_stack_size;

    /* Define pointers to the next and previous ready threads.  */
    struct ACE_OS_THREAD_STRUCT
                *ace_os_thread_ready_next,
                *ace_os_thread_ready_previous;
    
    CHAR        *ace_os_thread_name;
    UINT        ace_os_thread_priority;
    UINT        ace_os_thread_state;

    /* Define pointers to the next and previous threads in the
       created list.  */
    struct ACE_OS_THREAD_STRUCT
                *ace_os_thread_created_next,
                *ace_os_thread_created_previous;

    VOID        (*ace_os_thread_entry)(ULONG id);
    ULONG       ace_os_thread_entry_params;

}ACE_OS_THREAD;


/* Kernel Enter Function */
#define ace_os_kernel_enter         ace_os_initialize_kernel_enter


/*
************************************************************************************************************************
************************************************************************************************************************
*                           A C E - O S   E X T E R N A L   F U N C T I O N
************************************************************************************************************************
************************************************************************************************************************
*/

/* Declare the ace_os_application_define function as having C linkage. */

VOID ace_os_application_define(VOID *first_unused_memory);

/* Define the function prototypes of the ThreadX API. */

/* Define block memory pool management function prototypes. */

UINT ace_os_block_pool_create(ACE_OS_BLOCK_POOL *pool_ptr, CHAR *name_ptr, ULONG block_size,
                            VOID *pool_start, ULONG pool_size);


/* Define byte memory pool management function prototypes. */

UINT ace_os_byte_allocate(ACE_OS_BYTE_POOL *pool_ptr, VOID **memory_ptr, ULONG memory_size,
                         ULONG wait_option);
UINT ace_os_byte_pool_create(ACE_OS_BYTE_POOL *pool_ptr, CHAR *name_ptr, VOID *pool_start, 
                        ULONG pool_size);


/* Define event flags management function prototypes. */


/* Define initialization function prototyes. */

VOID ace_os_initialize_kernel_enter(VOID);


/* Define mutex management function prototypes. */


/* Define queue management function prototypes. */


/* Define semaphore management function prototypes. */


/* Define thread control function prototypes. */


/* Define timer management function prototypes. */


/*  */

/*
************************************************************************************************************************
************************************************************************************************************************
*                           A C E - O S   
************************************************************************************************************************
************************************************************************************************************************
*/

#define ACE_OS_MEMSET(a, b, c)                                 memset((a), (b), (c))

#define ACE_OS_UCHAR_POINTER_ADD(a, b)                         (((UCHAR *)(a)) + ((UINT)(b)))
#define ACE_OS_UCHAR_POINTER_SUB(a, b)                         (((UCHAR *)(a)) - ((UINT)(b)))
#define ACE_OS_ULONG_POINTER_ADD(a, b)                         (((ULONG *)(a)) + ((UINT)(b)))
#define ACE_OS_ULONG_POINTER_SUB(a, b)                         (((ULONG *)(a)) - ((UINT)(b)))
#define ACE_OS_UCHAR_POINTER_DIF(a, b)                         ((ULONG)(((UCHAR *) (a)) - ((UCHAR *) (b))))
#define ACE_OS_ULONG_POINTER_DIF(a, b)                         ((ULONG)(((ULONG *) (a)) - ((ULONG *) (b))))
/* Convert void between char */
#define ACE_OS_VOID_TO_UCHAR_POINTER_CONVERT(tmp)              ((UCHAR *) ((VOID *) (tmp)))
#define ACE_OS_UCHAR_TO_VOID_POINTER_CONVERT(tmp)              ((VOID *) (tmp))

#define ACE_OS_UCHAR_TO_INDIRECT_UCHAR_POINTER_CONVERT(temp)   ((UCHAR**) ((VOID *) (temp)))
#define ACE_OS_INDIRECT_VOID_TO_UCHAR_POINTER_CONVERT(temp)    ((UCHAR**) ((VOID *) (temp)))
#define ACE_OS_BLOCK_POOL_TO_UCHAR_POINTER_CONVERT(temp)       ((UCHAR *) ((VOID *) (temp)))
#define ACE_OS_UCHAR_TO_BLOCK_POOL_POINTER_CONVERT(temp)       ((ACE_OS_BLOCK_POOL *) ((VOID *) (temp)))
#define ACE_OS_VOID_TO_BYTE_POOL_POINTER_CONVERT(temp)         ((ACE_OS_BYTE_POOL *) ((VOID *) (temp)))
#define ACE_OS_BYTE_POOL_TO_UCHAR_POINTER_CONVERT(temp)        ((UCHAR *) ((VOID *) (temp)))
#define ACE_OS_UCHAR_TO_ALIGN_TYPE_POINTER_CONVERT(temp)       ((ALIGN_TYPE *) ((VOID *) (temp)))


#ifdef __cplusplus
}
#endif
#endif
