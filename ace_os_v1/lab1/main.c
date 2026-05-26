#include <stdio.h>

int main(void)
{
    unsigned tick = 0;

    for (;;) {
        printf("hello from lab1 over ITM: %u\n", tick++);
        for (volatile unsigned delay = 0; delay < 1000000U; ++delay) {
        }
    }
}
