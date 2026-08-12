

    .text
    .align 4
    .syntax unified

// VOID     ace_os_thread_stack_build(ACE_OS_THREAD *thread_ptr, VOID (*function_ptr)(VOID))
// {
    .global ace_os_thread_stack_build
    .thumb_func
ace_os_thread_stack_build:

    /* Stack Bottom : (higher memory address) */

    LDR     r2, [r0, #16]       // pickup end of stack area (ace_os_thread_stack_end)
    BIC     r2, r2, #0x07       // Align frame for 8-byte aligment
    SUB     r2, r2, #68         // Subtract frame size, step back 16 address
    LDR     r3, =0xFFFFFFFD     // Set initial LR value
    STR     r3, [r2, #0]        // Save on the stack

    /* Actually build the stack frame */

    MOV     r3, #0              // Build initial register value 0
    STR     r3, [r2, #4]        // Store initial r4
    STR     r3, [r2, #8]        // Store initial r5
    STR     r3, [r2, #12]       // Store initial r6
    STR     r3, [r2, #16]       // Store initial r7
    STR     r3, [r2, #20]       // Store initial r8
    STR     r3, [r2, #24]       // Store initial r9
    STR     r3, [r2, #28]       // Store initial r10
    STR     r3, [r2, #32]       // Store initial r11

    /* Hardware stack follows.  */

    STR     r3, [r2, #36]       // Store initial r0
    STR     r3, [r2, #40]       // Store initial r1
    STR     r3, [r2, #44]       // Store initial r2
    STR     r3, [r2, #48]       // Store initial r3
    STR     r3, [r2, #52]       // Store initial r12
    MOV     r3, #0xFFFFFFFF     // Poison EXC_RETURN value
    STR     r3, [r2, #56]       // Store initial lr
    STR     r1, [r2, #60]       // Store initial pc
    MOV     r3, #0x01000000     // Only T-bit need be set
    STR     r3, [r2, #64]       // Store initial xPSR

    /* Setup stack pointer */
    // thread_ptr -> tx_thread_stack_ptr =  r2;
    STR     r2, [r0, #8]        // Save stack pointer in thread's
                                // control block
    BX      lr                  // Return to caller

//}



