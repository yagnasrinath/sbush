#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<dirent.h>
void *opendir(const char *path)
{
    uint64_t fd = open(path,0);
    if(fd == -1)
    {
    	return NULL;
    }
    else
    {
    	//struct linux_dirent ld;
    	//uint64_t ret = syscall_3((uint64_t)SYS_getdents,(uint64_t)fd,(uint64_t)&ld,(uint64_t))
    }
    return NULL;
}
