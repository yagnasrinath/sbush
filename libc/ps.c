#include<stdlib.h>
#include<stdio.h>
#include<syscall.h>
void ps(){
    syscall_0((uint64_t)SYS_ps);
}


