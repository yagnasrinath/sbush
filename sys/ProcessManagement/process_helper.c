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

//init task_struct
task_struct *init_task_struct = NULL;

task_struct* new_task = NULL;
// current pid
int curr_pid ;



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
	//return FALSE;
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
		panic("FATAL: NULL task structure \n");
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


void free_process_vma_list(vma_struct* curr_list){
	vma_struct* free_list_head = curr_list;
	vma_struct* free_list_tail = NULL;
	while(curr_list != NULL){
		curr_list->vm_area_start = 0;
		curr_list->vm_area_end = 0;
		curr_list->vma_perm = NOPERM;
		curr_list->vma_type = NONE;
		//we have to free the file struct here
		curr_list->file_descp = 0;
		curr_list->parent_mem_struct = NULL;
		free_list_tail = curr_list;
		curr_list = curr_list->next;
	}
	free_list_tail->next = free_vma_list;
	free_vma_list = free_list_head;
}

void print_children(task_struct* task) {
	task_struct* children = task->children_head;
	kprintf("children are ");
	if(children == NULL) {
		kprintf("empty \n");
		return;
	}
	while(children != NULL) {
		kprintf(" %d ",children->pid );
		children = children->next_sibling;
	}


}

void detach_children(task_struct* parent_task_struct){

	task_struct* children = parent_task_struct->children_head;
	task_struct* children_head = children;
	task_struct* children_tail = NULL;
	//print_children(parent_task_struct);
	while(children != NULL){
		children->ppid = init_task_struct->pid;
		children->parent = init_task_struct;
		children_tail = children;
		children = children->next_sibling;
	}
	if(children_head != NULL) {
		children_tail->next_sibling = init_task_struct->children_head;
		init_task_struct->children_head = children_head;
	}
	parent_task_struct->children_head = NULL;
}


void remove_child(task_struct* child_task){
	task_struct* parent_task = child_task->parent;
	task_struct* sib = parent_task->children_head;
	task_struct* last_sib = NULL;

	while(sib!=NULL)
	{
		if(sib==child_task)
		{
			break;
		}
		last_sib = sib;
		sib=sib->next_sibling;
	}
	if(sib==NULL)
	{
		panic("child is not present in parent's children list");
	}
	if(last_sib!=NULL)
	{
		last_sib->next_sibling = sib->next_sibling;
	}
	else
	{
		parent_task->children_head= sib->next_sibling;
	}
	sib->next_sibling = NULL;
}


void detach_from_parent(task_struct* child_task_struct){
	task_struct* parent_task_struct = child_task_struct->parent;
	//kprintf("address of parent task struct is %p", parent_task_struct);
	if(parent_task_struct != NULL) {
		if(parent_task_struct->state == WAIT ) {
			if(parent_task_struct->wait_pid == child_task_struct->pid || parent_task_struct->wait_pid == -1){
				parent_task_struct->state = READY;
				//kprintf("parent marked ready %d\n", parent_task_struct->wait_pid);
				remove_child(child_task_struct);
				child_task_struct->state = EXIT;
			}
			else{
				child_task_struct->state = ZOMBIE;
			}
		}
		else{
			child_task_struct->state = ZOMBIE;
		}
	}
	else {
		panic("NO PARENT \n");
	}
	//kprintf("detached from parent\n");
}

void free_task_struct(task_struct* to_free){
	to_free->children_head = NULL;
	to_free->end = NULL;
	kmemset(to_free->fd,0,MAX_FD_PER_PROC*sizeof(uint64_t));
	to_free->is_user_proc = FALSE;

	//kprintf("value of kernel stack is %p \n",(uint64_t)to_free->kstack );

	kmemset(to_free->kstack, 0 , PAGE_SIZE);
	to_free->next = NULL;
	to_free->num_of_children = 0;
	to_free->pid = 0;
	to_free->ppid = 0;
	to_free->rsp  = 0;
	to_free->next_sibling = NULL;
	to_free->state = READY;
	kmemset(to_free->task_name,'\0',TASK_NAME_LENGTH);
	to_free->virtual_addr_space->brk_end = 0;
	to_free->virtual_addr_space->stack_start= 0;
	to_free->virtual_addr_space-> brk_start = 0;
	to_free->virtual_addr_space->vmaList = 0;
	//to_free->virtual_addr_space->pml4_t = 0;
	to_free->wait_pid = 0;
	add_free_task_struct(to_free);
}




task_struct* create_new_task(BOOL is_user_process) {

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
		new_task->next_sibling = NULL;
		kmemset(new_task->kstack, 0 , PAGE_SIZE);
		kmemset(new_task->fd,0,MAX_FD_PER_PROC*8);
	}
	else {
		new_task = get_free_task_struct();
		if(new_task == NULL) {
			panic("free task struct already allocated is NULL \n");
		}
	}
	new_task->is_user_proc = is_user_process;
	curr_pid = curr_pid+1;
	new_task->pid =curr_pid;
	new_task->state = READY;
	new_task->pid =curr_pid;
	new_task->ppid =0;
	__asm__ __volatile__("");
	new_task->pid =curr_pid;
	if(new_task->pid  == 0) {
		panic("new task pid is 0");
	}
	new_task->pid =curr_pid;
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
	if(new_vma == NULL) {
		panic("new VMA is NULL \n");
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


int get_curr_pid()
{
	return curr_pid;
}


