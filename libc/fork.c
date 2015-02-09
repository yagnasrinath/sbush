#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
pid_t fork(void)
{
	return syscall_0((uint64_t) SYS_fork);

}
