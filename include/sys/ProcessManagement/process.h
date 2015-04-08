/*
 * process.h
 *
 *  Created on: Apr 2, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_H_
#define SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_H_
// An Header File that holds all the data structures needed for the Process.

#include <sys/defs.h>

enum TASK_STATE {
	READY,
	RUNNING,
	WAIT,
	SLEEP,
	IDLE,
	ZOMBIE,
	EXIT
};


enum VMA_TYPE {
	ANON,
	TXT,
	FILE,
	STACK,
	HEAP,
	DATA,
	NONE
};

enum vma_perm {
	NONE, //0
	READ,//1
	WRITE, // 10
	READ_WRITE,// 11
	EXECUTE,//100
	EXECUTEREAD,//101
	EXECUTE_WRITE,//110
	EXECUTE_READ_WRITE //111
};

typedef struct mem_struct mem_struct;
typedef struct vm_area_struct vm_area_struct;
typedef struct task_struct task_struct;

struct {
	uint64_t pid;
	uint64_t ppid;
	uint64_t kstack[PAGE_SIZE/8];
	mem_struct* virtual_addr_space;
	enum TASK_STATE task_state;
	uint64_t rsp_register;
	char task_name[TASK_NAME_LENGTH];
	uint64_t sleep_for;
	BOOL is_user_proc;
	task_struct* parent;
	task_struct* children_head;
	task_struct* siblings;
	uint64_t wait_pid;
	uint64_t *fd[MAX_FD_PER_PROC];
	uint64_t  num_of_children;
	task_struct *next;
	task_struct *end;
}task_struct;

struct {
	vm_area_struct *vmaList;
	uint64_t stack_start, brk_start,brk_end;
	uint64_t pml4_t;
}mem_struct;

struct {
	uint64_t vm_area_start;
	uint64_t vm_area_end;
	enum VMA_TYPE vmatype;
	enum vma_perm vma_perm;
	uint64_t file_descp;
	vm_area_struct *next;
	mem_struct* parent_mem_struct;
}vm_area_struct;




#endif /* SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_H_ */
