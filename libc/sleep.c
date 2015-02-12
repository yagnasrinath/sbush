#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<string.h>
struct timespec{
        signed int tv_sec;
        long tv_nsec;
};
unsigned int sleep(unsigned int seconds){
        struct timespec req_time, rem_time;
        memset(&req_time,'\0', sizeof(req_time));
        memset(&rem_time, '\0', sizeof(rem_time));
        req_time.tv_sec = seconds;
        req_time.tv_nsec = 0;
        return syscall_2((uint64_t)SYS_nanosleep,(uint64_t)&req_time, (uint64_t)&rem_time);
}
