/*
 * idt.c
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/idt.h>
#include<sys/utils/string.h>
#define INTERRUPT 0x0e
struct idt_t {
	uint16_t offset_low;
	uint16_t selector;
	unsigned ist : 3 ;
	unsigned reserved0 : 5;
	unsigned type : 4;
	unsigned zero : 1;
	unsigned dpl : 2;
	unsigned p : 1;
	uint16_t offset_middle;
	uint32_t offset_high;
	uint32_t reserved1;
} __attribute__((packed));

void irq_remap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}


struct idtr_ptr {
	uint16_t size;
	uint64_t base;
}__attribute__((packed));


static struct idt_t idt_arr[256];
static struct idtr_ptr idt_ptr;

static void inline load_idt(void* idt_ptr) {
	__asm__ __volatile__(
			"lidt (%0)"
			:
			:"r" (idt_ptr)
	);
}


extern void x86_64_isr_vector0();
extern void x86_64_isr_vector32();

void idt_set_gate(uint16_t idtNum, uint64_t handler ) {
	idt_arr[idtNum].offset_low = handler & 0XFFFF;
	idt_arr[idtNum].offset_middle= handler >> 16 & 0XFFFF;
	idt_arr[idtNum].offset_high = handler >> 32 &0XFFFFFFFF;
	idt_arr[idtNum].selector = 8;
	idt_arr[idtNum].dpl = 0;
	idt_arr[idtNum].ist= 0;
	idt_arr[idtNum].type = INTERRUPT;
	idt_arr[idtNum].reserved0 =0;
	idt_arr[idtNum].reserved1 =0;
	idt_arr[idtNum].zero = 0;
	idt_arr[idtNum].p = 1;
}


void idt_install ()  {
	irq_remap();
	idt_ptr.size = (sizeof (struct idt_t) * 256) - 1;
	idt_ptr.base = (uint64_t)&idt_arr;
	memset(&idt_arr, 0, (sizeof (struct idt_t) * 256));
	// Load the ISR here
	idt_set_gate(0,(uint64_t)&x86_64_isr_vector0);


	//Load the hardware Interrupts here
	idt_set_gate(32,(uint64_t)&x86_64_isr_vector32);
	load_idt((void *)&idt_ptr);
}

