#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
int chdir(const char *path)
{
	int returnvalue =  syscall_1((uint64_t)SYS_chdir,(uint64_t)path);
    if(returnvalue < 0){
        errno = -(returnvalue);
        return -1;
    }
    return returnvalue;
}
