#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
int pipe(int filedes[2])
{
    return syscall_1((uint64_t)SYS_pipe,(uint64_t)filedes);
}
