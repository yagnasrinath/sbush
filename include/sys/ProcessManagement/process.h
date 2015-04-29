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
#include <sys/fs/fs.h>

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
	NOPERM, //0
	READ,//1
	WRITE, // 10
	READ_WRITE,// 11
	EXECUTE,//100
	EXECUTEREAD,//101
	EXECUTE_WRITE,//110
	EXECUTE_READ_WRITE,//111
	COPY_ON_WRITE //1000
};

typedef struct mem_structure mem_struct;
typedef struct vm_area_structure vma_struct;
typedef struct task_structure task_struct;

struct task_structure{
	int ppid;
	int pid;
	uint64_t kstack[KSTACK_SIZE];
	mem_struct* virtual_addr_space;
	enum TASK_STATE state;
	uint64_t rsp;
	char task_name[TASK_NAME_LENGTH];
	int sleep_for;
	BOOL is_user_proc;
	task_struct* parent;
	task_struct* children_head;
	task_struct* next_sibling;
	int wait_pid;
	file_des_t *fd[MAX_FD_PER_PROC];
	uint64_t  num_of_children;
	task_struct *next;
	task_struct *end;
}__attribute__((packed));

struct mem_structure{
	vma_struct *vmaList;
	uint64_t stack_start, brk_start,brk_end;
	uint64_t pml4_t;
};

struct vm_area_structure{
	uint64_t vm_area_start;
	uint64_t vm_area_end;
	enum VMA_TYPE vma_type;
	enum vma_perm vma_perm;
	uint64_t file_descp;
	vma_struct *next;
	mem_struct* parent_mem_struct;
};



task_struct* copy_task_struct(task_struct* parent_task_struct);
#endif /* SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_H_ */
