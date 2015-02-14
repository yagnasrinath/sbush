#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
pid_t waitpid(pid_t pid, int *status, int options)
{
    pid_t returnvalue = syscall_4((uint64_t) SYS_wait4, (uint64_t) pid, (uint64_t) status, (uint64_t) options,(uint64_t) NULL) ;
    if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}

