/*
 * system.h
 *
 *  Created on: Feb 28, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_SYSTEM_H_
#define SBUSH_INCLUDE_SYS_SYSTEM_H_
#include<sys/defs.h>
void timer_install();


inline unsigned char inportb (unsigned short _port) {
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

inline void outportb (unsigned short _port, unsigned char _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
inline void set_rsp(uint64_t rsp) {
	__asm__ __volatile__("movq %0, %%rsp" : : "r"(rsp));
}

#endif /* SBUSH_INCLUDE_SYS_SYSTEM_H_ */
