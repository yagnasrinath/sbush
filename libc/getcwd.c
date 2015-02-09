#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
char *getcwd(char *buf, size_t size)
{
	return (char *)syscall_2((uint64_t)SYS_getcwd,(uint64_t)buf,(uint64_t)size);
}
