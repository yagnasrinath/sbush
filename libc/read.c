#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
ssize_t read(int fd, void *buf, size_t count)
{
    uint64_t returnvalue = syscall_3((uint64_t)SYS_read,(uint64_t)fd,(uint64_t)buf, (uint64_t)count);
    if(returnvalue < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}

