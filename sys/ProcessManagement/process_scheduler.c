/*
 * process_scheduler.c
 *
 *  Created on: Apr 9, 2015
 *      Author: ravichandrasadineni
 */
#include<sys/defs.h>
#include<sys/system.h>
#include<sys/utils/kstring.h>
#include<sys/ProcessManagement/process.h>
#include<sys/ProcessManagement/process_helper.h>

extern void x86_64_isr32();
static task_struct* task_list;
static task_struct* curr_proc;
static task_struct* idle_process;

void initialize_proc_scheduler() {
	task_list = NULL;
	curr_proc= NULL;
	idle_process = NULL;
}

task_struct*  get_curr_task() {

	return curr_proc;
}

void awake_sleeping_proc() {
	task_struct *curr=task_list;
	while(curr != NULL) {
		if(curr->state == SLEEP) {
			curr->sleep_for -= 1;
			if(curr->sleep_for  < 0) {
				curr->state = READY;
			}
		}
		curr= curr->next;
	}
}

void add_to_task_list(task_struct * new_proc) {
	if(new_proc->state  == EXIT) {
		add_free_task_struct(new_proc);
		return;
	}
	if(new_proc->state  == IDLE ){
		return;
	}
	if(new_proc->state == RUNNING) {
		new_proc->state = READY;
	}

	task_struct *curr_ready_tasks  = task_list;
	if(curr_ready_tasks == NULL) {
		task_list = new_proc;
		return;
	}
	while(curr_ready_tasks->next != NULL) {
		curr_ready_tasks = curr_ready_tasks->next;
	}
	curr_ready_tasks->next = new_proc;

}

task_struct * get_next_ready_proc() {
	task_struct * next_ready_task = task_list;
	task_struct *  prev = NULL;
	while(next_ready_task != NULL) {
		if(next_ready_task->state == READY) {
			next_ready_task->state = RUNNING;
			break;
		}
		prev = next_ready_task;
		next_ready_task= next_ready_task->next;
	}
	if(next_ready_task == NULL) {

		next_ready_task = idle_process;
	}
	else {
		if(prev == NULL) {

			task_list= task_list->next;
		}
		else {
			prev->next = next_ready_task->next;
			next_ready_task->next = task_list;
		}
	}
	curr_proc = next_ready_task;

	return next_ready_task;
}

void schedule_process(task_struct * new_task, uint64_t stk_top, uint64_t entry) {
	// setting up  stack segemnt

	kprintf("process %s loaded \n", new_task->task_name);

	if(new_task->is_user_proc) {

		new_task->kstack[KSTACK_SIZE-1] = 0x23;
	} else {
		new_task->kstack[KSTACK_SIZE-1] = 0x10;
	}
	// setting up code segemnt
	if(new_task->is_user_proc) {
		new_task->kstack[KSTACK_SIZE-4] = 0x1b;
	} else {
		new_task->kstack[KSTACK_SIZE-4] = 0x08;
	}
	new_task->kstack[KSTACK_SIZE-5] = entry;
	new_task->kstack[KSTACK_SIZE-3] = 0x200282UL;
	new_task->kstack[KSTACK_SIZE-2] = stk_top;
	// PUSHA pushes the 15 general purpose registers here(kernel stack 6 to 20)
	// setting the return address to the POPA of x86_64_isr32
	new_task->kstack[KSTACK_SIZE-21] = (uint64_t)x86_64_isr32 + 0x20;
	//	new_task->rsp = (uint64_t)&new_task->kstack[KSTACK_SIZE-22];
	new_task->rsp  = (uint64_t)&new_task->kstack[KSTACK_SIZE-24];
	add_to_task_list(new_task);

}


static void idle_proc(void ) {
	kprintf("In the idle process \n");
	while(1) ;
}

void create_idle_proc() {
	idle_process = create_new_task(FALSE);
	idle_process->state = IDLE;
	kstrcpy(idle_process->task_name, "IDLE PROCESS");
	kprintf("new process kernel stack is %p",(uint64_t)&idle_process->kstack[KSTACK_SIZE-1]);
	schedule_process(idle_process,(uint64_t)&idle_process->kstack[KSTACK_SIZE-1],(uint64_t)idle_proc );
}
