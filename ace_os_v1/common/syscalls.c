#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include "stm32f746.h"

int _write(int file, char *ptr, int len)
{
    (void)file;

    for (int i = 0; i < len; ++i) {
        if (ptr[i] == '\n') {
            ITM_SendChar('\r');
        }
        ITM_SendChar(ptr[i]);
    }

    return len;
}

int _close(int file)
{
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    errno = EIO;
    return -1;
}
