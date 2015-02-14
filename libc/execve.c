#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<syscall.h>
int execve(const char *filename, char *const argv[], char *const envp[])
{
    uint64_t returnvalue = syscall_3((uint64_t)SYS_execve, (uint64_t) filename, (uint64_t) argv, (uint64_t) envp);
    if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return -1;
    }       
    return returnvalue;
}
