/*
 * system.h
 *
 *  Created on: Feb 28, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_SYSTEM_H_
#define SBUSH_INCLUDE_SYS_SYSTEM_H_
#include<sys/types.h>
void cls();
void putch(unsigned char c);
void puts(char *str);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
void init_video();


inline unsigned char inportb (unsigned short _port) {
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

inline void outportb (unsigned short _port, unsigned char _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

#endif /* SBUSH_INCLUDE_SYS_SYSTEM_H_ */
