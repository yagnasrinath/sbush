/*
 * process_helper.c
 *
 *  Created on: Apr 8, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/MemoryManagement/MemoryManagementUtil.h>
#include<sys/MemoryManagement/kmalloc.h>
#include<sys/defs.h>
#include <sys/sbunix.h>
#include <sys/ProcessManagement/process.h>
#include "../../include/sys/utils/kstring.h"


// free list of task structures
static task_struct *free_task_list;

// free list of vma list
static  vma_struct  *free_vma_list;


// current pid
uint64_t curr_pid ;

void initialize_free_list() {
	free_task_list = NULL;
	free_vma_list = NULL;
	curr_pid =0;
}

// useful when doing execve on an existing process.
void decrement_pid () {
	if(curr_pid !=0) {
		curr_pid  -=1;
	}
	else {
		kprintf("ERROR : ILLEGAL Process ID \n");
	}
}

BOOL is_free_task_struct_available () {
	return (free_task_list != NULL);
}

BOOL is_free_vma_struct_available () {
	return (free_vma_list != NULL);
}

task_struct * get_free_task_struct () {
	if(!is_free_task_struct_available()) {
		return NULL;
	}
	else {
		task_struct* next_free_struct = free_task_list;
		free_task_list = free_task_list->next;
		return next_free_struct;
	}
}

vma_struct *get_free_vma_struct() {
	if(!is_free_vma_struct_available()) {
		return NULL;
	}
	else {
		vma_struct* next_free_struct = free_vma_list;
		free_vma_list = free_vma_list->next;
		return next_free_struct;
	}
}


void add_free_task_struct (task_struct * new_task_struct) {
	if(new_task_struct == NULL) {
		kprintf("FATAL: NULL task structure \n");
	}
	new_task_struct->next = free_task_list;
	free_task_list = new_task_struct;
	return;
}


void add_free_vma_struct (vma_struct* new_vma_struct) {
	if(new_vma_struct == NULL) {
		kprintf("FATAL: NULL vma structure \n");
	}
	new_vma_struct->next = free_vma_list;
	free_vma_list = new_vma_struct;
}




task_struct* create_new_task(BOOL is_user_process) {
	task_struct* new_task = NULL;
	if(!is_free_task_struct_available()) {
		new_task = (task_struct*)kmalloc(sizeof(task_struct));
		mem_struct* mm_struct = (mem_struct*)kmalloc(sizeof(mem_struct));
		mm_struct->pml4_t = get_new_pml4_t();
		mm_struct->vmaList = NULL;
		new_task->virtual_addr_space = mm_struct;
		new_task->next = NULL;
		new_task->wait_pid = 0;
		new_task->end = NULL;
		new_task->children_head = NULL;
		new_task->num_of_children = 0;
		new_task->parent = NULL;
		new_task->siblings = NULL;
		kmemset(new_task->kstack, 0 , PAGE_SIZE);
		kmemset(new_task->fd,0,MAX_FD_PER_PROC*8);
	}
	else {
		new_task = get_free_task_struct();
	}
	new_task->is_user_proc = is_user_process;
	new_task->pid = ++curr_pid;
	new_task->state = READY;
	new_task->ppid =0;
	return new_task;
}

vma_struct* create_new_vma(uint64_t file_desc, uint64_t vma_start, uint64_t vma_end, uint64_t vma_type, uint64_t vma_perm) {
	vma_struct* new_vma;
	if(!is_free_vma_struct_available()) {
		new_vma= (vma_struct*)kmalloc(sizeof(vma_struct));
	}
	else {
		new_vma = get_free_vma_struct();
	}
	new_vma->file_descp = file_desc;
	new_vma->next = NULL;
	new_vma->parent_mem_struct = NULL;
	new_vma->vm_area_start = vma_start;
	new_vma->vm_area_end = vma_end;
	new_vma->vma_type = vma_type;
	new_vma->vma_perm = vma_perm;
	return new_vma;
}





