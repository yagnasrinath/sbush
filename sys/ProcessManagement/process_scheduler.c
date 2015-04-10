/*
 * process_scheduler.c
 *
 *  Created on: Apr 9, 2015
 *      Author: ravichandrasadineni
 */
#include<sys/defs.h>
#include<sys/utils/kstring.h>
#include<sys/ProcessManagement/process.h>
#include<sys/ProcessManagement/process_helper.h>

extern void x86_64_isr32();
static task_struct* task_list = NULL;
//static task_struct* curr_task = NULL;

void awake_sleeping_proc() {
	task_struct *curr_proc=task_list;
	while(curr_proc != NULL) {
		if(curr_proc->task_state == SLEEP) {
			curr_proc->sleep_for -= 1;
			if(curr_proc->sleep_for  < 0) {
				curr_proc->task_state = READY;
			}
		}
		curr_proc= curr_proc->next;
	}
}

void add_to_task_list(task_struct * new_proc) {
	if(new_proc->task_state  == EXIT) {
		add_free_task_struct(new_proc);
		return;
	}
	if(new_proc->task_state  == IDLE ){
		return;
	}
	if(new_proc->task_state == RUNNING) {
		new_proc->task_state = READY;
	}

	task_struct *curr_ready_tasks  = task_list;
	while(curr_ready_tasks != NULL) {
		curr_ready_tasks = curr_ready_tasks->next;
	}
	if(curr_ready_tasks == NULL) {
		curr_ready_tasks = new_proc;
	}
	curr_ready_tasks->next = new_proc;
}


void schedule_process(task_struct * new_task, uint64_t stk_top, uint64_t entry) {
	// setting up  stack segemnt
	if(new_task->is_user_proc) {
		new_task->kstack[KSTACK_SIZE-1] = 0x23;
	} else {
		new_task->kstack[KSTACK_SIZE-1] = 0x10;
	}
	// setting up data segemnt
	if(new_task->is_user_proc) {
		new_task->kstack[KSTACK_SIZE-4] = 0x1b;
	} else {
		new_task->kstack[KSTACK_SIZE-4] = 0x08;
	}
	new_task->kstack[KSTACK_SIZE-5] = entry;
	new_task->kstack[KSTACK_SIZE-3] = 0x200202UL;
	new_task->kstack[KSTACK_SIZE-2] = stk_top;
	// PUSHA pushes the 15 general purpose registers here(kernel stack 6 to 20)
	// setting the return address to the POPA of x86_64_isr32
	new_task->kstack[KSTACK_SIZE-21] = (uint64_t)x86_64_isr32 + 0x20;
	add_to_task_list(new_task);

}


