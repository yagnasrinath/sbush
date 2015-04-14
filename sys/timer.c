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
#define switch_to_ring3 \
		__asm__ __volatile__(\
				"movq $0x23, %rax;"\
				"movq %rax,  %fs;"\
				"movq %rax,  %es;"\
				"movq %rax,  %ds;"\
				"movq %rax,  %gs;")

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
void timer_handler(struct isr_nrm_regs r)
{

	timer_ticks++;
	if (timer_ticks % 100 == 0)
	{
		numOfsecs++;
		printtimeatrightconer(numOfsecs);
	}
	prev = get_curr_task();
	awake_sleeping_proc();
	if(prev == NULL) {
		next= get_next_ready_proc();

		_set_cr3(next->virtual_addr_space->pml4_t);


		set_rsp(next->rsp);
		if(next->is_user_proc) {
			reload_tss((uint64_t)(&(next->kstack[KSTACK_SIZE-1])));
			switch_to_ring3;
		}
	}else {
		register uint64_t curr_rsp __asm__("rsp");
		prev ->rsp = curr_rsp;
		add_to_task_list(prev);
		next = get_next_ready_proc();
		if(prev !=next) {
			_set_cr3(next->virtual_addr_space->pml4_t);
			set_rsp(next->rsp);
			if(next->is_user_proc) {
				reload_tss((uint64_t)(&(next->kstack[KSTACK_SIZE-1])));
				switch_to_ring3;
			}
		}
	}
	outportb(0x20, 0x20);
}


void timer_install() {
	uint32_t divisor = 1193180;
	outportb(0x43, 0x36);
	outportb(0x40, divisor & 0xFF);
	outportb(0x40, divisor >> 8);
}
