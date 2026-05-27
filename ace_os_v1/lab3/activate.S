.thumb
.syntax unified


.global activate
.type activate, %function
activate:
    // save old stack pointer
    mrs ip, psr
    push {r4-r11, ip, lr}

    // switch to user task stack
    ldmia r0!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    msr psp, r0
    @ // Use PSP, but keep privileged mode so ITM/debug registers are accessible.
    @ // CONTROL bit 1 = SPSEL, bit 0 = nPRIV.
    @ movs r0, #2
    @ msr CONTROL, r0
    @ isb

    bx lr

.global SVC_Handler
.type SVC_Handler, %function
SVC_Handler:
    // Save context of the user task
    mrs r0, psp
    stmdb r0!, {r4-r11, lr} // Save callee-saved registers

    // 
    pop {r4-r11, ip, lr} // Get syscall number and arguments (if any)

    bx lr

.global task_init_env
.type task_init_env, %function
task_init_env:
    // Save context of the user task
    mrs ip, psr
    push {r4-r11, ip, lr}

    msr psp, r0 // Update PSP with the new stack pointer after saving context
    mov r0, #2
    msr CONTROL, r0 // Switch to unprivileged mode with PSP
    isb
    bl syscall // Call syscall to trigger SVC_Handler
    bx lr

