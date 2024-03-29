#ifndef _DEFS_H
#define _DEFS_H

#define NULL ((void*)0)

typedef unsigned long   size_t;
typedef unsigned long   uint64_t;
typedef          long   int64_t;
typedef unsigned int    uint32_t;
typedef          int    int32_t;
typedef unsigned short  uint16_t;
typedef          short  int16_t;
typedef unsigned char uint8_t;
#define TRUE  1
#define FALSE 0
#define PAGE_SIZE 4096
#define KSTACK_SIZE 512   // since it is an array of uint64_t 8*12
#define USR_STK_SIZE 0x10000 // 64KB *
#define TASK_NAME_LENGTH 50
#define MAX_FULL_PATH_LEN 100
#define  MAX_FD_PER_PROC 12
#define PAGE_ALIGN(ADDR) ((ADDR) >> 12 << 12)
#define KERNEL_RW_FLAG 2UL  // 10
#define KERNEL_RX_FLAG 0UL  //0
#define USER_RW_FLAG 6UL  //110
#define USER_RX_FLAG 4UL   //100
#define PAGE_PRESENT 1UL   // 1
#define RW_FLAG 2UL //2
#define UNSET_FLAGS   0x0000FFFFFFFFF000UL
#define FLAGS         0x0000000000000FFFUL
#define USR_STK_TOP  0xF0000000000UL
#define MAX_USR_ARG_LEN 100
#define MAX_ARGS 10
#define MAX_ENVP 10
#define MAX_USR_ENVP_LEN 100

typedef int BOOL;

#define MAX_FILE_NAME_LEN 50
#define MAX_PATH_LEN 256
#define MAXCHILD 12
#define PIPE_SIZE 4024
struct dirent {
    char d_name[MAX_FILE_NAME_LEN];
};

#endif

