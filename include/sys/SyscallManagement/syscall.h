/*
 * syscall.h
 *
 *  Created on: Apr 19, 2015
 *      Author: harsha
 */

#ifndef INCLUDE_SYS_SYSCALLMANAGEMENT_SYSCALL_H_
#define INCLUDE_SYS_SYSCALLMANAGEMENT_SYSCALL_H_

#include<sys/idt.h>

void handle_syscall(struct isr_nrm_regs regs);

#endif /* INCLUDE_SYS_SYSCALLMANAGEMENT_SYSCALL_H_ */
