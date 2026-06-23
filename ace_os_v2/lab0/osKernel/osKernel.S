.thumb
.syntax unified


.extern currentPt

.global SysTick_Handler
.type SysTick_Handler, %function
SysTick_Handler:
    cpsid i
    push [R4 - R11]
    ldr  

