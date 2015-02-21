#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include<syscall.h>
#include<errno.h>
ssize_t write(int fd, const void *buf, size_t count){
    ssize_t returnvalue = syscall_3((uint64_t)SYS_write,(uint64_t)fd,(uint64_t)buf, (uint64_t)count);
    if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}


