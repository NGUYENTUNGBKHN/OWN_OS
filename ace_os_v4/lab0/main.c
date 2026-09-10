#include <stdio.h>
#include "ace_os_api.h"

#define DEMO_STACK_SIZE         1024
#define DEMO_BYTE_POOL_SIZE     9120

ACE_OS_THREAD               thread_0;
ACE_OS_THREAD               thread_1;
ACE_OS_BYTE_POOL            byte_pool_0;
UCHAR   memory_area[DEMO_BYTE_POOL_SIZE];

ULONG   thread_0_counter;
ULONG   thread_1_counter;

void    thread_0_entry(ULONG thread_input);
void    thread_1_entry(ULONG thread_input);

int main(void)
{
    printf("ACE_OS_V4 start !!\n");

    ace_os_kernel_enter();
    /* Enter the ThreadX kernel. */
    // for (;;) {
    // }
}

VOID ace_os_application_define(VOID *first_unused_memory)
{
    CHAR    *pointer;

    (void) first_unused_memory;

    ace_os_byte_pool_create(&byte_pool_0, "byte_pool_0", memory_area, DEMO_BYTE_POOL_SIZE);

    ace_os_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, ACE_OS_NO_WAIT);

    ace_os_thread_create(&thread_0, "thread 0", thread_0_entry, 0,
            pointer, DEMO_STACK_SIZE,
            1, 1, 0, ACE_OS_AUTO_START);

    ace_os_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, ACE_OS_NO_WAIT);

    ace_os_thread_create(&thread_1, "thread 1", thread_1_entry, 1,
            pointer, DEMO_STACK_SIZE,
            16, 16, 4, ACE_OS_AUTO_START);
}

void    thread_0_entry(ULONG thread_input)
{

    UINT    status = ACE_OS_SUCCESS;

    (void) thread_input;

    /* This thread simply sits in while-forever-sleep loop.  */
    while(1)
    {

        /* Increment the thread counter.  */
        thread_0_counter++;

        /* Sleep */
        ace_os_thread_sleep(10);

        /* Check status.  */
        if (status != ACE_OS_SUCCESS)
            break;
    }
}


void    thread_1_entry(ULONG thread_input)
{

    UINT    status = ACE_OS_SUCCESS;

    (void) thread_input;

    /* This thread simply sits in while-forever-sleep loop.  */
    while(1)
    {

        /* Increment the thread counter.  */
        thread_1_counter++;

        /* Check status.  */
        if (status != ACE_OS_SUCCESS)
            break;
    }
}

