

    .text
    .align 4
    .syntax unified

// VOID     ace_os_thread_stack_build(ACE_OS_THREAD *thread_ptr, VOID (*function_ptr)(VOID))
// {
    .global ace_os_thread_stack_build
    .thumb_func
ace_os_thread_stack_build:

    /* Stack Bottom : (higher memory address) */

    LDR     r2, [r0, #16]       // pickup end of stack area (ace_os_thread_stack_start)
    BIC     r2, r2, #0x07       

//}