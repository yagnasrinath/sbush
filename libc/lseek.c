#include<stdlib.h>
#include<stdio.h>
#include<syscall.h>
#include<errno.h>
off_t lseek(int fildes, off_t offset, int whence){
    uint64_t returnvalue = syscall_3((uint64_t)SYS_lseek, (uint64_t)fildes, (uint64_t)offset, (uint64_t)whence);
    if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}
