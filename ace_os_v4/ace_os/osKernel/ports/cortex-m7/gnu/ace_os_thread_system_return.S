
#ifdef ACE_OS_INCLUDE_USER_DEFINE_FILE
#include "ace_os_user.h"
#endif

    .text   32
    .align 4
    .syntax unified

// VOID ace_os_thread_system_return(VOID)
// {
    .thumb_func
    .global ace_os_thread_system_return
ace_os_thread_system_return:

    /* Return to real scheduler via PendSV. Note that this routine is often
        replaced with in-line assembly in tx_port.h to impoved performance. */
        
    MOV     r0, #0x10000000                         // Load PENDSVSET bit
    MOV     r1, #0xE000E000                         // Load NVIC base
    STR     r0, [r1, #0xD04]                        // Set PENDSVBIT in ICSR
    DSB     #0xF                                    // Ensure memory access is complete
    ISB     #0xF                                    // Flush pipeline
    MRS     r0, IPSR                                // Pickup IPSR
    CMP     r0, #0                                  // Is it a thread returning?
    BNE     _isr_context                            // If ISR, skip interrupt enable

    MRS     r1, PRIMASK                             // Thread context returning, pickup PRIMASK
    CPSIE   i                                       // Enable interrupts
    MSR     PRIMASK, r1                             // Restore original interrupt posture
    ISB     #0xF                                    // Flush pipeline

_isr_context:
    BX      lr                                      // Return to caller
// }



