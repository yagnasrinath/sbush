#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/defs.h>
#include <sys/syscall.h>

static __inline uint64_t syscall_0(uint64_t n) {
	uint64_t ret;
	__asm__ __volatile__
	(
			"int $0x80"
			:"=a"(ret)
			 : "0"(n)
			   : "cc", "%rcx", "%r11", "memory","%rdi","%rsi","%rbp","%rbx","%r8","%r9","%r10","%r12","%r13","%r14","%r15","%rdx"
	);
	return ret;
}

static __inline uint64_t syscall_1(uint64_t n, uint64_t a1) {
	uint64_t ret;
	__asm__ __volatile__
	(
			"int $0x80"
			: "=a" (ret)
			: "0"(n), "D"(a1)
			: "cc", "%rcx", "%r11", "memory","%rsi","%rbp","%rbx","%r8","%r9","%r10","%r12","%r13","%r14","%r15","%rdx"
	);

	return ret;


}

static __inline uint64_t syscall_2(uint64_t n, uint64_t a1, uint64_t a2) {
	uint64_t ret;
	__asm__ __volatile__
	(
			"int $0x80"
			: "=a" (ret)
			: "0"(n), "D"(a1), "S"(a2)
			: "cc", "%rcx", "%r11", "memory","%rbp","%rbx","%r8","%r9","%r10","%r12","%r13","%r14","%r15","%rdx"
	);

	return ret;


}

static __inline uint64_t syscall_3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
	uint64_t ret;
	__asm__ __volatile__
	(
			"int $0x80"
			: "=a" (ret)
			: "0"(n), "D"(a1), "S"(a2), "d"(a3)
			: "cc", "%rcx", "%r11", "memory","%rbp","%rbx","%r8","%r9","%r10","%r12","%r13","%r14","%r15"
	);

	return ret;
}


static __inline uint64_t syscall_4(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3,uint64_t a4) {
	uint64_t ret;
	register uint64_t a4r10 __asm("r10") = a4;
	__asm__ __volatile__
	(
			"int $0x80"
			: "=a" (ret)
			: "0"(n), "D"(a1), "S"(a2), "d"(a3), "r"(a4r10)
			: "cc", "%rcx", "%r11", "memory","%rbp","%rbx","%r8","%r9","%r12","%r13","%r14","%r15"
	);

	return ret;
}

#endif
