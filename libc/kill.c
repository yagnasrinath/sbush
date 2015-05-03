#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
int kill(int sig_no, int pid)
{
    uint64_t returnvalue = syscall_2((uint64_t)SYS_kill,(uint64_t)sig_no,(uint64_t)pid);
    if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}
