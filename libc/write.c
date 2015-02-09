#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include<syscall.h>





ssize_t write(int fd, const void *buf, size_t count){
	ssize_t ret;
	asm volatile
	(
			"syscall"
			: "=a" (ret)
			: "0"(SYS_write), "D"(fd), "S"(buf), "d"(count)
			: "cc", "rcx", "r11", "memory"
	);
	return ret;
}

