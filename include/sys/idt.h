/*
 * idt.h
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_IDT_H_
#define SBUSH_INCLUDE_SYS_IDT_H_

#include <sys/system.h>

void idt_set_gate(uint16_t idtNum, uint64_t handler );
void idt_install();



#endif /* SBUSH_INCLUDE_SYS_IDT_H_ */
