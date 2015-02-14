#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
int close(int fd)
{
	uint64_t returnvalue = syscall_1((uint64_t)SYS_close,(uint64_t)fd);
	  if(((int)returnvalue) < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}
