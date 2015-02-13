#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
int pipe(int filedes[2])
{
    int returnvalue = syscall_1((uint64_t)SYS_pipe,(uint64_t)filedes);
    if(returnvalue < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}

