#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
int open(const char *pathname, int flags)
{
    int returnvalue = syscall_2((uint64_t)SYS_open,(uint64_t)pathname,(uint64_t)flags);
    if(returnvalue < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}

