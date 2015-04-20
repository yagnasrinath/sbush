/*
 * syscall.c
 *
 *  Created on: Apr 19, 2015
 *      Author: harsha
 */
#include<sys/idt.h>
#include<sys/sbunix.h>
#include<sys/ProcessManagement/process_scheduler.h>
uint64_t* sys_calls[20];

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





/*uint64_t fork()  {
	task_struct * curr_task = get_curr_task();
	task_struct * child_task = copy_task_struct(curr_task);
	schedule_process(child_task, curr_task->kstack[KSTACK_SIZE-2], curr_task->kstack[KSTACK_SIZE-5]);
	return
}*/





void handle_syscall() {
	PUSHA;
	task_struct * curr_task = get_curr_task();
	curr_task->kstack[KSTACK_SIZE-8] = 12234;
	uint64_t  ret =0;
	kprintf("syscall %s\n", ret);
	POPA;
	__asm__ __volatile__("addq $0x08, %rsp");
	__asm__ __volatile__("iretq");

}

