#include<stdlib.h>
#include<stdio.h>
#include<syscall.h>
pid_t getpid(void){
    return syscall_0((uint64_t)SYS_getpid);
}
