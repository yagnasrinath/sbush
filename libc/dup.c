#include<stdlib.h>
#include<stdio.h>
#include<syscall.h>
int dup(int oldfd){
    return syscall_1(SYS_dup,oldfd);
}
