.thumb
.syntax unified


.global activate
.type activate, %function
activate:
    // save old stack pointer
    mrs ip, psr
    push {r4-r11, ip, lr}

    // switch to user task stack
    msr psp, r0
    // Use PSP, but keep privileged mode so ITM/debug registers are accessible.
    // CONTROL bit 1 = SPSEL, bit 0 = nPRIV.
    movs r0, #2
    msr CONTROL, r0
    isb
    // return to user task
    pop {r4-r11, lr}

    bx lr


