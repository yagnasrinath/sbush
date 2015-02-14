#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
char *getcwd(char *buf, size_t size)
{
    uint64_t returnvalue = syscall_2((uint64_t)SYS_getcwd,(uint64_t)buf,(uint64_t)size);
    if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return 0;
    }
    return (char*)returnvalue;
}

