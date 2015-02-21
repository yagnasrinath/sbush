#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<syscall.h>

int getdirentries(int fd, struct dirent* direntry , int size )
{
    int returnvalue = syscall_3((uint64_t)SYS_getdents, (uint64_t) fd, (uint64_t) direntry, (uint64_t) size);
    if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}
