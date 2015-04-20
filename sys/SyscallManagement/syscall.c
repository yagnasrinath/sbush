/*
 * syscall.c
 *
 *  Created on: Apr 19, 2015
 *      Author: harsha
 */
#include<sys/idt.h>
#include<sys/sbunix.h>

void handle_syscall(struct isr_nrm_regs regs) {


	kprintf("syscall\n");


}

