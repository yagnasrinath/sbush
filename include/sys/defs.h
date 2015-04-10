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
#define KSTACK_SIZE 512   // since it is an array of uint64_t
#define TASK_NAME_LENGTH 50
#define  MAX_FD_PER_PROC 8
#define PAGE_ALIGN(ADDR) ((ADDR) >> 12 << 12)
typedef int BOOL;

#endif

