#include<stdlib.h>
#include<stdio.h>
#include<syscall.h>
#include<errno.h>
int dup(int oldfd){
    uint64_t returnvalue = syscall_1(SYS_dup,oldfd);
    if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}
