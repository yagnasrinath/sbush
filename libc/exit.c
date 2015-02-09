#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include<syscall.h>
void exit(int status) {
	syscall_1((uint64_t)SYS_exit,(uint64_t)status);
}

