

#ifdef TX_INCLUDE_USER_DEFINE_FILE
#include "ace_os_user.h"
#endif

    .text
    .align 4
    .syntax unified

    .global ace_os_timer_system_clock
    .global ace_os_timer_time_slice
    .global ace_os_timer_expired_time_slice

    
// VOID ace_os_timer_interrupt(VOID)
// {
    .global ace_os_timer_interrupt
    .thumb_func
ace_os_timer_interrupt:

    /* Upon entry to this routinem it is assumed that the compiler scratch resgister are available 
        for use.  */
    
    /* Increment the system clock.  */
    // ace_os_timer_system_clock++;

    LDR     r1, =ace_os_timer_system_clock          // Pickup address of system clock
    LDR     r0, [r1, #0]                            // Pickup system clock
    ADD     r0, r0, #1                              // Increment system clock
    STR     r0, [r1, #0]                            // Store new system clock

    /* Test for time-slice expiration. */
    // if (ace_os_timer_time_slice)
    // {
    LDR     r3, =ace_os_timer_time_slice            // Pickup address of time slice
    LDR     r2, [r3, #0]                            // Pickup time slice
    CBZ     r2, __ace_os_timer_no_slice             // jump to time_slice = 0
                                                    // Yes, skip time-slice processing
    /* Decrement the time_slice */
    // ace_os_timer_time_slice--;

    SUB     r2, r2, #1                              // Decrement the time-slice
    STR     r2, [r3, #0]                            // Store new time-slice value

    /* Check for expiration. */
    // if (ace_os_timer_time_slice == 0)
    
    CBNZ    r2, __ace_os_timer_no_time_slice        // Has it expired?

    /* Set the time slice expired flag. */
    // ace_os_timer_expired_time_slice = ACE_OS_TRUE;

    LDR     r3, =ace_os_timer_expired_time_slice    // Pickup address of expired flag
    MOV     r0, #1
    STR     r0, [r3, #0]                            // Store ace_os_timer_expired_time_slice

    // }

__ace_os_timer_no_slice:

    /* Test for timer expiration. */
    // if (*ace_os_timer_current_ptr)
    // {

    LDR     r1, =ace_os_timer_current_ptr       // Pickup current timer pointer address
    LDR     r0, [r1, #0]                        // Pickup current timer
    LDR     r2, [r0, #0]                        // pickup timer list entry

    // }
    // else
    // {

// }
