#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
int chdir(const char *path)
{
	return syscall_1((uint64_t)SYS_chdir,(uint64_t)path);
}
