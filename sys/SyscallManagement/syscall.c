/*
 * syscall.
 *
 *  Created on: Apr 19, 2015
 *      Author: harsha
 */
#include<sys/idt.h>
#include<sys/sbunix.h>
#include<sys/utils/kstring.h>
#include<sys/ProcessManagement/process_scheduler.h>
#include<sys/ProcessManagement/process.h>


#define POPA \
		__asm__ __volatile__(\
				"popq %r15;"\
				"popq %r14;"\
				"popq %r13;"\
				"popq %r12;"\
				"popq %r11;"\
				"popq %r10;"\
				"popq %r9;"\
				"popq %r8;"\
				"popq %rsi;"\
				"popq %rbp;"\
				"popq %rdx;"\
				"popq %rcx;"\
				"popq %rbx;"\
				"popq %rax;"\
				"popq %rdi;")

#define PUSHA \
		__asm__ __volatile__(\
				"pushq %rdi;"\
				"pushq %rax;"\
				"pushq %rbx;"\
				"pushq %rcx;"\
				"pushq %rdx;"\
				"pushq %rbp;"\
				"pushq %rsi;"\
				"pushq %r8;"\
				"pushq %r9;"\
				"pushq %r10;"\
				"pushq %r11;"\
				"pushq %r12;"\
				"pushq %r13;"\
				"pushq %r14;"\
				"pushq %r15;")





void fork()  {
	task_struct * curr_task = get_curr_task();
	task_struct * child_task = copy_task_struct(curr_task);
	kmemset(child_task->kstack, 0 , PAGE_SIZE);
	kprintf("pml4t of child is %p \n",child_task->virtual_addr_space->pml4_t );
	kprintf("pml4t of parent is %p \n",curr_task->virtual_addr_space->pml4_t );
	schedule_process(child_task, curr_task->kstack[KSTACK_SIZE-4], curr_task->kstack[KSTACK_SIZE-7]);
	 curr_task->kstack[KSTACK_SIZE-10] = child_task->pid;
}

/*void* sys_calls[20] = {NULL,NULL,NULL,NULL,NULL,
						   NULL,fork,NULL,NULL,NULL,
						   NULL,NULL,NULL,NULL,NULL,
						   NULL,NULL,NULL,NULL,NULL};*/




void handle_syscall() {
	PUSHA;
	task_struct* curr_task = get_curr_task();
	if(curr_task->kstack[KSTACK_SIZE-10] <0 || curr_task->kstack[KSTACK_SIZE-10] >19) {
		kprintf("Invalid interrupt number \n");
	}
	else {
	 switch(curr_task->kstack[KSTACK_SIZE-10]) {
		 case 7 : fork(); break;
	 	 default : break;
		}
	}
	POPA;
	__asm__ __volatile__("addq $0x08, %rsp;");
	__asm__ __volatile__("iretq");

}

