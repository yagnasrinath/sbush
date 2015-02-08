#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
int execve(const char *filename, char *const argv[], char *const envp[])
{
    printf("%d\n",errno);       
    return 0;
}
