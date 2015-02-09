#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<syscall.h>
int execve(const char *filename, char *const argv[], char *const envp[])
{
	syscall_3((uint64_t)SYS_execve, (uint64_t) filename, (uint64_t) argv, (uint64_t) envp);

    printf("%d\n",errno);       
    return 0;
}
