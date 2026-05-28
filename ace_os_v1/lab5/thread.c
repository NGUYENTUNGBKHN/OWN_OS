/**
 * @file       thread.c
 * @brief      
 * @date       2026/05/27
 * @author     [Gentantun] (nguyenthanhtung8196@gmail.com)
 * @details    
 * @ref        
 * @copyright  Copyright (c) 2026 RoboTun
*/

/*******************************************************************************
**                                INCLUDES
*******************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include "thread.h"
/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/

#define THREAD_PSP          0xFFFFFFFD
#define SCB_ICSR            ((volatile uint32_t *)0xE000ED04)
#define SCB_ICSR_PENDSVSET  (1UL << 28)
/*******************************************************************************
**                      COMMON VARIABLE DEFINITIONS
*******************************************************************************/


/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
typedef struct THREAD_CONTROL_BLOCK_S 
{
    void *stack_pointer;            // Pointer to the top of the thread's stack
    void *origin_stack_pointer;     // Pointer to the original stack base (for cleanup)
    uint8_t in_use;                 // Flag to indicate if the TCB is currently in use
}tcb_t;

/*******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
static tcb_t thread[2];
static int last_thread = 0;

/*******************************************************************************
**                          FUNCTION DEFINITIONS
*******************************************************************************/
void __attribute__((naked)) PendSV_Handler()
{
    /* Save the context of the current thread */
    __asm volatile("mrs r0, psp\n"                     // Get the current thread's stack pointer
                 "stmdb r0!, {r4-r11, lr}\n");       // Push callee-saved registers and LR onto the stack

    __asm volatile("mov %0, r0\n" : "=r" (thread[last_thread].stack_pointer)); // Save the updated stack pointer to the current thread's TCB

    while (1)
    {
        last_thread++;
        if (last_thread >= 2) {
            last_thread = 0; // Wrap around to the first thread
        }
        if (thread[last_thread].in_use) {
            __asm volatile("mov r0, %0\n" : : "r" (thread[last_thread].stack_pointer)); // Load the stack pointer of the next thread
            __asm volatile("ldmia r0!, {r4-r11, lr}\n"
			             "msr psp, r0\n"
			             "bx lr\n");
        }
    }
    
}


void SysTick_Handler()
{
    // Active pendSV to trigger a context switch
    *SCB_ICSR |= SCB_ICSR_PENDSVSET;
}

void __attribute__((naked)) thread_start()
{
    last_thread = 0; // Start with the first thread
    /* Clear the APSR flags */
    __asm volatile("mov r0, #0\n"
                 "msr APSR_nzcvq, r0\n");
    /* Load the stack pointer of the first thread and start executing */
    __asm volatile("mov r0, %0\n" : : "r" (thread[0].stack_pointer)); // Load the stack pointer of the first thread
    __asm volatile("msr psp, r0\n"
                 "mov r0, #2\n"                     // Set CONTROL register to use PSP and unprivileged mode
                 "msr CONTROL, r0\n");              // Load the CONTROL register
    __asm volatile("isb\n"                            // Instruction Synchronization Barrier to ensure the changes take effect
                 "pop {r4-r11, lr}\n"               // Restore callee-saved registers
                 "ldr r0, [sp]\n");                 // Load the PC from the stack (the task function address) and start executing

    __asm volatile("ldr pc, [sp, #24]\n");

    while (1); // Should never reach here
}

int thread_create(void (*task_func)(void*), void *userdata)
{
    uint32_t *stack;
    // Find an available TCB slot
    int tcb_index = -1;
    for (int i = 0; i < 2; i++) {
        if (!thread[i].in_use) {
            tcb_index = i;
            break;
        }
    }
    
    if (tcb_index == -1) {
        return -1; // No available TCB slots
    }

    stack = malloc(STACK_SIZE * sizeof(unsigned int)); // Allocate stack memory for the thread
    if (stack == NULL) {
        return -1; // Memory allocation failed
    }   
    // Initialize the TCB
    thread[tcb_index].stack_pointer = stack + STACK_SIZE - 17; // Set stack pointer to the top of the stack
    thread[tcb_index].origin_stack_pointer = stack; // Store original stack base for cleanup
    thread[tcb_index].in_use = 1; // Mark TCB as in use

    // Set up the initial stack frame for the new thread
    unsigned int *stack_frame = (unsigned int *)thread[tcb_index].stack_pointer;
    
    stack_frame[8] = THREAD_PSP;                // LR (EXC_RETURN to Thread mode, use PSP)
    stack_frame[9] = (unsigned int)userdata;    // R0 (argument to the task function)
    stack_frame[14] =  (unsigned int)&thread_self_terminal;   // R12 (Return address for the task function, points to self-termination function)
    stack_frame[15] = (unsigned int)task_func;  // PC (Program Counter) points to the task function
    stack_frame[16] = 0x01000000;               // xPSR (Thumb state)

    return tcb_index; // Return the index of the created thread
}



void thread_kill(int thread_id)
{
	thread[thread_id].in_use = 0;

	/* Free the stack */
	free(thread[thread_id].origin_stack_pointer);
}

void thread_self_terminal()
{
	/* This will kill the stack.
	 * For now, disable context switches to save ourselves.
	 */
	__asm volatile("cpsid i\n");
	thread_kill(last_thread);
	__asm volatile("cpsie i\n");

	/* And now wait for death to kick in */
	while (1);
}



/******************************** End of file *********************************/

