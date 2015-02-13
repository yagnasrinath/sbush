#include<stdlib.h>
#include<stdio.h>
#include<syscall.h>
unsigned int alarm(unsigned int seconds){
    return syscall_1((uint64_t)SYS_alarm, (uint64_t)seconds); 
}


