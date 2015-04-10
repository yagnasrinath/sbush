/*
 * kb_intrpt_handler.h
 *
 *  Created on: Mar 2, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_KB_INTRPT_HANDLER_H_
#define SBUSH_INCLUDE_SYS_KB_INTRPT_HANDLER_H_
#include<sys/idt.h>
void  kb_intrpt_handler( struct isr_nrm_regs  stack);

#endif /* SBUSH_INCLUDE_SYS_KB_INTRPT_HANDLER_H_ */
