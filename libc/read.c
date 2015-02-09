#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
ssize_t read(int fd, void *buf, size_t count)
{
    return syscall_3((uint64_t)SYS_read,(uint64_t)fd,(uint64_t)buf, (uint64_t)count);
}
