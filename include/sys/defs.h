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
#define  MAX_FD_PER_PROC 8
#define PAGE_ALIGN(ADDR) ((ADDR) >> 12 << 12)
#define KERNEL_RW_FLAG 2UL  // 10
#define KERNEL_RX_FLAG 0UL  //0
#define USER_RW_FLAG 6UL  //110
#define USER_RX_FLAG 4UL   //100
#define PAGE_PRESENT 1UL   // 1
#define PHY_PAGE_BITS 0x000FFFFFFFFFF000UL
#define USR_STK_TOP  0xF0000000000UL
#define MAX_USR_ARG_LEN 100
#define MAX_ARGS 10

typedef int BOOL;

#endif

