
@********************************************************************************************************
@                                          PUBLIC FUNCTIONS
@********************************************************************************************************

@ External Reference
    .extern ace_os_curr_ptr

    .global ace_os_pendSV_handler
    .global ace_os_intCtxSw
    .global ace_os_ctxSw
    .global ace_os_start_rdy

@********************************************************************************************************
@                                               EQUATES
@********************************************************************************************************

.equ NVIC_INT_CTRL,     0xE000ED04                              @ Interrupt control state register.
.equ NVIC_SYSPRI14,     0xE000ED22                              @ System priority register (priority 14).
.equ NVIC_PENDSV_PRI,   0xFF                                    @ PendSV priority value (lowest).
.equ NVIC_PENDSVSET,    0x10000000                              @ Value to trigger PendSV exception.


@********************************************************************************************************
@                                     CODE GENERATION DIRECTIVES
@********************************************************************************************************
    .text
    .align 2
    .thumb
    .syntax unified

.thumb_func
ace_os_start_rdy:
    CPSID   I
    MOVW    R0, #:lower16:NVIC_SYSPRI14    @ Set the PendSV exception priority
    MOVT    R0, #:upper16:NVIC_SYSPRI14

    MOVW    R1, #:lower16:NVIC_PENDSV_PRI
    MOVT    R1, #:upper16:NVIC_PENDSV_PRI
    STRB    R1, [R0]

    MOVW    R0, #:lower16:ace_os_curr_ptr
    MOVT    R0, #:upper16:ace_os_curr_ptr
    LDR     R2, [R0]

    LDR     R0, [R2]
    MSR     PSP, R0

    MRS     R0, CONTROL
    ORR     R0, R0, #2
    BIC     R0, R0, #4                    @ Clear FPCA bit to indicate FPU is not in use
    MSR     CONTROL, R0
    ISB                                   @ Sync instruction stream

    LDMFD    SP!, {R4-R11, LR}            @ Restore r4-11, lr from new process stack
    LDMFD    SP!, {R0-R3}                 @ Restore r0, r3
    LDMFD    SP!, {R12, LR}               @ Load R12 and LR
    LDMFD    SP!, {R1, R2}                @ Load PC and discard xPSR
    CPSIE   I
    BX       R1


.thumb_func
ace_os_ctxSw:
.thumb_func
ace_os_intCtxSw:
    LDR     R0, =NVIC_INT_CTRL      @ Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR



.thumb_func
ace_os_pendSV_handler:
    @ Save status of old task
    MRS     R0, PSP                 @ PSP is process stack pointer
    STMFD   R0!, {R4-R11, R14}      @ Save remaining regs r4-11, R14 on process stack

    MOVW    R1, #:lower16:ace_os_curr_ptr    @ ace_os_curr_ptr = ace_os_curr_ptr;
    MOVT    R1, #:upper16:ace_os_curr_ptr
    LDR     R2, [R1]

    ORR     LR,  R4, #0x04                   @ Ensure exception return uses process stack
    LDR     R0, [R2]                         @ R0 is new process SP; SP = OSTCBHighRdyPtr->StkPtr;
    LDMFD   R0!, {R4-R11, R14}               @ Restore r4-11, R14 from new process stack

    MSR     PSP, R0                          @ Load PSP with new process SP

    MOV     R2, #0                           @ Restore BASEPRI priority level to 0
    BX      LR


.end
