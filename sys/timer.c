/*
 * timer.c
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/idt.h>
#include<sys/sbunix.h>
#include<sys/scrn.h>
#include<sys/system.h>
#include<sys/gdt.h>
#include<sys/ProcessManagement/process_scheduler.h>
int timer_ticks =0;
int numOfsecs = 0;
task_struct* next;
task_struct* prev = NULL;
//BOOL INITSCHEDULING= FALSE;
#define LOAD_CR3(lcr3) __asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(lcr3));
#define switch_to_ring3 \
		__asm__ __volatile__(\
				"movq $0x23, %rax;"\
				"movq %rax,  %ds;"\
				"movq %rax,  %es;"\
				"movq %rax,  %fs;"\
				"movq %rax,  %gs;")

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
				"popq %rdi;"\
				"iretq;")

#define PUSHA \
		__asm__ __volatile__(\
				"pushq %r15;"\
				"pushq %r14;"\
				"pushq %r13;"\
				"pushq %r12;"\
				"pushq %r11;"\
				"pushq %r10;"\
				"pushq %r9;"\
				"pushq %r8;"\
				"pushq %rsi;"\
				"pushq %rbp;"\
				"pushq %rdx;"\
				"pushq %rcx;"\
				"pushq %rbx;"\
				"pushq %rax;"\
				"pushq %rdi;")



extern void _set_cr3(uint64_t pml4);



void printtimeatrightconer(int value) {

	int sec_length =0;
	char output[33];
	char *ptr;
	ptr = &output[32];
	*ptr = '\0';
	char numberbase[] = "0123456789abcdef";
	ptr--;
	do {
		int currnum = value%10;
		*ptr =numberbase[currnum];
		ptr --;
		sec_length++;
		value = value/10;
	}while(value!=0);
	ptr++;
	putsat(24,  78-sec_length, ptr);
	return ;

}
void timer_handler()
{
	//PUSHA;
	timer_ticks++;
	if (timer_ticks % 100 == 0)
	{
		timer_ticks =0;
		numOfsecs++;
		printtimeatrightconer(numOfsecs);
	}
	//if(INITSCHEDULING) {
	prev = get_curr_task();
	//awake_sleeping_proc();
	if(prev == NULL) {

		next= get_next_ready_proc();
		kprintf("Process loaded is %s \n", next->task_name);

		//_set_cr3(next->virtual_addr_space->pml4_t);

		LOAD_CR3(next->virtual_addr_space->pml4_t);
		__asm__ __volatile__("movq %[next_rsp], %%rsp" : : [next_rsp] "m" (next->rsp));
		if(next->is_user_proc) {
			reload_tss((uint64_t)(&(next->kstack[KSTACK_SIZE-1])));
			switch_to_ring3;
		}
	}else {
		uint64_t cur_rsp;
		__asm__ __volatile__("movq %%rsp, %[cur_rsp]": [cur_rsp] "=r"(cur_rsp));
		kprintf("prev process is %s \n", prev->task_name);
		prev ->rsp = cur_rsp;
		add_to_task_list(prev);
		next = get_next_ready_proc();
		if(prev !=next) {
			//_set_cr3(next->virtual_addr_space->pml4_t);

			//kprintf("CR3 register is %p \n", next->virtual_addr_space->pml4_t);

			LOAD_CR3(next->virtual_addr_space->pml4_t);

			__asm__ __volatile__("movq %[next_rsp], %%rsp" : : [next_rsp] "m" (next->rsp));

			if(next->is_user_proc) {
				reload_tss((uint64_t)(&(next->kstack[KSTACK_SIZE-1])));
				switch_to_ring3;
			}
		}

	}
	outportb(0x20, 0x20);
	//POPA;


}


void timer_install() {
	uint32_t divisor = 1193180;
	outportb(0x43, 0x36);
	outportb(0x40, divisor & 0xFF);
	outportb(0x40, divisor >> 8);
}
