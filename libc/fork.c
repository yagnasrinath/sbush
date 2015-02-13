#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
pid_t fork(void)
{
    pid_t returnvalue = syscall_0((uint64_t) SYS_fork);
    if(returnvalue < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}

