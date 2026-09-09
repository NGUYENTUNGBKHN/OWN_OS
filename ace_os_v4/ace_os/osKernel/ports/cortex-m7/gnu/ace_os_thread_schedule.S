

#ifdef ACE_OS_INCLUDE_USER_DEFINE_FILE
#include "ace_os_user.h"
#endif

    .text
    .align 4
    .syntax unified

    .global ace_os_thread_current_ptr
    .global ace_os_thread_execute_ptr
    .global ace_os_timer_time_slice


// VOID ace_os_thread_schedule(VOID)
// {
    .global ace_os_thread_schedule
    .thumb_func
ace_os_thread_schedule:
    /* This function should only ever be called on Cortex-M
        from the first schedule request. Subsequent scheduling occurs
        from the PendSV handing routine below. */
    

    /* Clear the preempt-disable flag to enable rescheduling after initialization
        on target Cortex-M. */

    /* Clear CONTROL.FPCA bit sp VFP resgiters aren't unnecessarily stacked. */


    /* Enable interrupt */
    CPSIE       i

    /* Enter the scheduler for the first time. */

    MOV     r0, #0x10000000             // Load PENDSVSET bit
    MOV     r1, #0xE000E000             // Load NVIC base
    STR     r0, [r1, #0xD04]            // Set PENDSVBIT in ICSR
    DSB                                 // Complete all memory accesses
    ISB                                 // Flush pipeline

    /* Wait here for the PendSV to take place.  */
__ace_os_wait_here:
    B   __ace_os_wait_here              // Wait for the PendSV to happen

// }

    /* Generic context switching PendSV handler.  */
// VOID PendSV_IRQHandler(VOID)
// {
    .global  PendSV_IRQHandler
    .global  __ace_os_PendSVHandler
    .syntax  unified
    .thumb_func
PendSV_IRQHandler:
    .thumb_func
__ace_os_PendSVHandler:
    /* Get current thread value and new thread pointer.  */

__ace_os_ts_handler:

    LDR     r0, =ace_os_thread_current_ptr          // Build current thread pointer address
    LDR     r2, =ace_os_thread_execute_ptr          // Build execute thread pointer address
    MOV     r3, #0                                  // Build NULL value
    LDR     r1, [r0]                                // Pickup current thread pointer

    /* Determine if there is a current thread to finish preserving. */

    CBZ     r1, __ace_os_ts_new                     // If NULL, skip preservation

    /* Recover PSP and preserve current thread context. */

    STR     r3, [r0]                                // Set ace_os_thread_current_ptr to NULL
    MRS     r12, PSP                                // Pickup PSP pointer (thread's stack pointer)
    STMDB   r12!, {r4-r11}                          // Save its remaining registers

    /* vfp */
    /* vfp */

    LDR     r4, =ace_os_timer_time_slice            // Build address of time-slice
    STMDB   r12!, {LR}                              // Save LR on the stack

    /* Determine if time-slice is active. IF it isn't, skip time handling processing. */

    LDR     r5, [r4]                                // Pickup current time-slice
    STR     r12, [r1, #8]                           // Save the thread stack pointer
    CBZ     r5, __ace_os_ts_new                     // If not active, skip proccessing

    /* Time-slice is active, save the current thread's time-slice and clear the global time-slice variable.  */

    STR     r5, [r1, #24]                           // Save current time-slice

    /* Clear the global time-slice.  */

    STR     r3, [r4]                                // Clear time-slice

    /* Executing thread is now completely preserved!!!  */

__ace_os_ts_new:

    /* Now we are looking for a new thread to execute! */

    CPSID   i                                       // Disable interrupts

    LDR     r1, [r2]                                // Is there another thread ready to execute?
    CBZ     r1, __ace_os_ts_wait                    // Skip to the wait processing

    /* Another thread is ready for else, make the current thread the new thread. */

    STR     r1, [r0]                                // Setup the currrent thread

    CPSIE   i                                       // Enable interrupts

    /* Increment the thread run count.  */

__ace_os_ts_restore:
    LDR     r7, [r1, #4]
    LDR     r4, =ace_os_timer_time_slice
    LDR     r5, [r1, #24]
    ADD     r7, r7, #1
    STR     r7, [r1, #4]

    /* Setup global time-slice with thread's current time-slice. */

    STR     r5, [r4]

    /* Restore the thread context and PSP  */

    LDR     r12, [r1, #8]                           // Pickup thread's stack pointer
    LDMIA   r12!, {LR}                              // Pickup LR

    /* VFP */
    /* VFP */

    LDMIA   r12!, {r4-r11}                          // Recover thread's registers
    MRS     PSP, r12                                // Setup the thread's stack pointer

    /* Return to thread */

    BX      lr                                      // Return to thread!

    /* The following is the idle wait processing... in this case, no threads are ready for execution and the
       system will simply be idle until an interrupt occurs that makes a thread ready. Note that interrupts
       are disabled to allow use of WFI for waiting for a thread to arrive. */

__ace_os_ts_wait:
    CPSID   i                                       // Disable interrupts

    LDR     r1, [r2]                                // Pickup the next thread to execute pointer
    STR     r1, [r0]                                // Store it in the current pointer
    CBNZ    r1, __ace_os_ts_ready                   // If non-NULL, a new thread is ready!

    CPSIE   i                                       // Enable interrupts

    B       __ace_os_ts_wait                        // Loop to continue waiting

    /* At this point, we have a new thread ready to go. Clear any newly pended PendSV - since we are
       already in the handler!  */
    
__ace_os_ts_ready:
    MOV     r7, #0x08000000                         // Build clear PendSV value
    MOV     r8, #0xE000E000                         // Build base NVIC address
    STR     r7, [r8, #0xD04]                        // Clear any PendSV

    CPSIE   i                                       // Enable interrupts

    B       __ace_os_ts_restore                     // Restore the thread

// }


/* VFP */
/* VFP */
