.thumb
.syntax unified

.global syscall
.type syscall, %function
syscall:
    svc 0
    nop
    bx lr

    