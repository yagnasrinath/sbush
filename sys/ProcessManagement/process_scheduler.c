/*
 * process_scheduler.c
 *
 *  Created on: Apr 9, 2015
 *      Author: ravichandrasadineni
 */
#include<sys/defs.h>

#include<sys/ProcessManagement/process.h>
#include<sys/ProcessManagement/process_helper.h>

extern void x86_64_isr32();
//static task_struct* task_list = NULL;
//static task_struct* prev_task = NULL;
//static task_struct* next_task = NULL;
//static task_struct* curr_task = NULL;
static task_struct* idle_process = NULL;


//static void idle_proc(void ) {
//	kprintf("In idle process \n");
//	while(1);
//}

void create_idle_proc() {
	idle_process = create_new_task(FALSE);
	idle_process->task_state = IDLE;


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
	// setting the return address to the POPA of time_handler
	new_task->kstack[KSTACK_SIZE-21] = (uint64_t)x86_64_isr32 + 0x20;

}

