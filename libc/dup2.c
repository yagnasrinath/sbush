#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
int dup2(int oldfd, int newfd)
{
    return syscall_2((uint64_t)SYS_dup2,(uint64_t)oldfd,(uint64_t)newfd);;
}
