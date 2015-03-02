/*
 * isr.c
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */
/* adapted from Chris Stones, shovelos */
#include <sys/sbunix.h>
#include <sys/stdlib.h>
#include <sys/isr.h>
#include<sys/system.h>


#define INTERRUPT(vector) \
	__asm__(".global x86_64_isr_vector" #vector "\n"\
			"x86_64_isr_vector" #vector ":\n" \
			"    pushq %rax;" \
			"    pushq %rcx;" \
			"    pushq %rdx;" \
			"    pushq %rsi;" \
			"    pushq %rdi;" \
			"    pushq %r8;" \
			"    pushq %r9;" \
			"    pushq %r10;" \
			"    pushq %r11;" \
			"    movq  %rsp,%rdi;" \
			"    addq  $72, %rdi;"  \
			"    call x86_64_handle_isr_vector" #vector ";" \
			"    popq %r11;" \
			"    popq %r10;" \
			"    popq %r9;" \
			"    popq %r8;" \
			"    popq %rdi;" \
			"    popq %rsi;" \
			"    popq %rdx;" \
			"    popq %rcx;" \
			"    popq %rax;" \
	"iretq;")




INTERRUPT(0);
INTERRUPT(32);
INTERRUPT(33);
// hardware interrupts
void *irq_routines[16] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

// Install a new handler for a Hardware interrupt
void irq_install_handler(int irq, void (*handler)(struct isr_nrm_regs *stack))
{
    irq_routines[irq] = handler;
}
// uninstall a new hardware for a Hardware Interrupt
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

void x86_64_handle_isr_vector0(struct isr_nrm_regs *stack) {
	printf("devided by zero exception\n");
	printf("CS:0x%x\n",stack->cs);
	printf("RIP:0x%x\n",stack->rip);
	HALT("");
}

void x86_64_handle_isr_vector32(struct isr_nrm_regs *stack) {
	void (*handler)(struct isr_nrm_regs *stack);
	handler =irq_routines[0];
	handler(stack);
	// send the EOI to the master controller.
	outportb(0x20, 0x20);
}

void x86_64_handle_isr_vector33(struct isr_nrm_regs *stack) {
	void (*handler)(struct isr_nrm_regs *stack);
	handler =irq_routines[1];
	handler(stack);
	// send the EOI to the master controller.
	outportb(0x20, 0x20);
}


