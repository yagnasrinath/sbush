/*
 * isr.h
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_ISR_H_
#define SBUSH_INCLUDE_SYS_ISR_H_
#include<sys/defs.h>

struct isr_err_regs
{
	uint64_t err;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
};


struct isr_nrm_regs
{
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
};

void irq_install_handler(int irq, void (*handler)(struct isr_nrm_regs *stack));
void irq_uninstall_handler(int irq);

#endif /* SBUSH_INCLUDE_SYS_ISR_H_ */
