#include<stdlib.h>
#include<stdio.h>
#include<syscall.h>
off_t lseek(int fildes, off_t offset, int whence){
    return syscall_3((uint64_t)SYS_lseek, (uint64_t)fildes, (uint64_t)offset, (uint64_t)whence);
}
