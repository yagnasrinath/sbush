#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
int dup2(int oldfd, int newfd)
{
    uint64_t returnvalue = syscall_2((uint64_t)SYS_dup2,(uint64_t)oldfd,(uint64_t)newfd);
    if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}
