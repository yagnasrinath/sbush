/*
 * system.h
 *
 *  Created on: Feb 28, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_SYSTEM_H_
#define SBUSH_INCLUDE_SYS_SYSTEM_H_
#include<sys/defs.h>
#define IA32_MSR_EFER 0xC0000080
#define IA32_MSR_STAR 0xC0000081
#define IA32_MSR_LSTAR 0xC0000082
#define IA32_MSR_CSTAR 0xC0000083
#define IA32_MSR_FMASK 0xC0000084

void timer_install();



static inline unsigned char inportb (unsigned short _port) {
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

static inline void outportb (unsigned short _port, unsigned char _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

static inline void set_rsp(uint64_t rsp) {
	__asm__ __volatile__("movq %0, %%rsp" : : "r"(rsp));
}


static inline uint64_t read_cr3() {
	uint64_t cr3;
	__asm__ __volatile__ ("movq %%cr3, %0" : "=a" (cr3):);
	return cr3;
}

static inline void set_cr3(uint64_t cr3) {
	__asm__ __volatile__("movq %0, %%cr3" : : "r"(cr3));
}


static inline void set_cr4()
{
	__asm__ __volatile__("mov %cr4, %eax\n\t"
			"bts $5, %eax\n\t"
			"mov %eax, %cr4"
			:
			:
			:"eax");
}


static inline void set_star_MSR()
{
	__asm__ __volatile__(
			"mov $0xC0000081, %ecx;"
			"rdmsr;"
			"mov $0x001b0008, %edx;"
			"xor %eax, %eax;"
			"wrmsr;"
	);
}



static inline void get_MSR(uint64_t msr, uint32_t *lo, uint32_t *hi)
{
	__asm__ __volatile__("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

static inline void set_MSR(uint64_t msr, uint32_t lo, uint32_t hi)
{
	__asm__ __volatile__("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

#endif /* SBUSH_INCLUDE_SYS_SYSTEM_H_ */
