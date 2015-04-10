/*
 * idt.h
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_IDT_H_
#define SBUSH_INCLUDE_SYS_IDT_H_

#include <sys/system.h>
struct isr_nrm_regs
{
	 uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rbp, rdx, rcx, rbx, rax, rdi;
	 uint64_t interrupt, error;
	 uint64_t rip, cs, rflags, rsp, ss;
};
void idt_set_gate(uint16_t idtNum, uint64_t handler );
void idt_install();



#endif /* SBUSH_INCLUDE_SYS_IDT_H_ */
