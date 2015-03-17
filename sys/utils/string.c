/*
 * string.c
 *
 *  Created on: Feb 27, 2015
 *      Author: ravichandrasadineni
 */
#include<sys/utils/string.h>

void *memcpy(void *dest, const void *source, int count){
	char *des = (char *) dest;
	const char* src = (char *)source;

	for(;count;count--){
		*des++ =*src++;
	}

	return dest;
}

void *memset(void *dest, char val, size_t count) {
	char *dst = (char *) dest;
	for(;count;count--){
		*dst++ = val;
	}
	return dest;
}
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count) {
	unsigned short * dst = (unsigned short *)dest;
	for(;count;count--) {
		*dst++ = val;
	}
	return dest;
}

int strlen(const char *str) {
	int count =0;
	while(*str++) count++;
	return count;
}

