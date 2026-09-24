

    .global     ace_os_thread_system_stack_ptr
    .global     ace_os_initialize_unused_memory
    .global     __RAM_segment_used_end__
    .global     ace_os_timer_interrupt
    .global     systick_cycles
    .global     _isr_vector

    .text   32
    .align  4
    .syntax unified


// VOID ace_os_initialize_low_level(VOID)
// {
    .global ace_os_initialize_low_level
    .thumb_func
ace_os_initialize_low_level:

    /* Disable interrupt during ThreadX initialization.  */
    CPSID   i

    /* Set base of available memory to end of non-initialised RAM areq.  */
    /* Set base of available memory to end of non-initialised RAM area.  */
    LDR     r0, =ace_os_initialize_unused_memory       // Build address of unused memory pointer
    LDR     r1, =__RAM_segment_used_end__           // Build first free address
    ADD     r1, r1, #4                              //
    STR     r1, [r0]                                // Setup first unused memory pointer

    /* Setup Vector Table Offset Register.  */
    MOV     r0, #0xE000E000                         // Build address of NVIC registers
    LDR     r1, =_isr_vector                           // Pickup address of vector table
    STR     r1, [r0, #0xD08]                        // Set vector table address

    /* Set system stack pointer from vector value.  */
    LDR     r0, =ace_os_thread_system_stack_ptr     // Build address of system stack pointer
    LDR     r1, =_isr_vector                         // Pickup address of vector table
    LDR     r1, [r1]                                // Pickup reset stack pointer
    STR     r1, [r0]                                // Save system stack pointer

    /* Enable the cycle count register.  */
    LDR     r0, =0xE0001000                         // Build address of DWT register
    LDR     r1, [r0]                                // Pickup the current value
    ORR     r1, r1, #1                              // Set the CYCCNTENA bit
    STR     r1, [r0]                                // Enable the cycle count register

    /* Configure SysTick.  */
    MOV     r0, #0xE000E000                         // Build address of NVIC registers
    LDR     r1, =systick_cycles
    STR     r1, [r0, #0x14]                         // Setup SysTick Reload Value
    MOV     r1, #0x7                                // Build SysTick Control Enable Value
    STR     r1, [r0, #0x10]                         // Setup SysTick Control

    /* Configure handler priorities.  */
    LDR     r1, =0x00000000                         // Rsrv, UsgF, BusF, MemM
    STR     r1, [r0, #0xD18]                        // Setup System Handlers 4-7 Priority Registers
    LDR     r1, =0xFF000000                         // SVCl, Rsrv, Rsrv, Rsrv
    STR     r1, [r0, #0xD1C]                        // Setup System Handlers 8-11 Priority Registers
                                                    // Note: SVC must be lowest priority, which is 0xFF
    LDR     r1, =0x40FF0000                         // SysT, PnSV, Rsrv, DbgM
    STR     r1, [r0, #0xD20]                        // Setup System Handlers 12-15 Priority Registers
                                                    // Note: PnSV must be lowest priority, which is 0xFF

    /* Return to caller.  */
    BX      lr
// }



/* SystemTick timer interrupt handler  */
    .global SysTick_IRQHandler
    .thumb_func
SysTick_IRQHandler:
// VOID SysTick_Handler(VOID)
// {
    PUSH    {r0, lr}

    @ BL      ace_os_timer_interrupt

    POP     {r0, lr}
    BX      lr

// }
