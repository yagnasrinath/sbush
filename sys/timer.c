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
#include<sys/ProcessManagement/process_scheduler.h>
int timer_ticks =0;
int numOfsecs = 0;

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

	awake_sleeping_proc();
	task_struct* next = NULL;
	//task_struct* prev = NULL;
	if(get_curr_task() == NULL) {
		next= get_next_ready_proc();
		kprintf("rsp register is %p", next->rsp_register);
		_set_cr3(next->virtual_addr_space->pml4_t);
		set_rsp(next->rsp_register);
	}
	outportb(0x20, 0x20);
}


void timer_install() {
		uint32_t divisor = 1193180;
		outportb(0x43, 0x36);
		outportb(0x40, divisor & 0xFF);
		outportb(0x40, divisor >> 8);
}

#define switch_to_ring3 \
    __asm__ __volatile__(\
        "movq $0x23, %rax;"\
        "movq %rax,  %ds;"\
        "movq %rax,  %es;"\
        "movq %rax,  %fs;"\
        "movq %rax,  %gs;")
