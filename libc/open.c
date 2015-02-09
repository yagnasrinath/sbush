#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
int open(const char *pathname, int flags)
{
    return syscall_2((uint64_t)SYS_open,(uint64_t)pathname,(uint64_t)flags);
}
