/*
 * string.c
 *
 *  Created on: Feb 27, 2015
 *      Author: ravichandrasadineni
 */
#include "../../include/sys/utils/kstring.h"

void *kmemcpy(void *dest, const void *source, int count){
	char *des = (char *) dest;
	const char* src = (char *)source;

	for(;count;count--){
		*des++ =*src++;
	}

	return dest;
}

void *kmemset(void *dest, char val, size_t count) {
	char *dst = (char *) dest;
	for(;count;count--){
		*dst++ = val;
	}
	return dest;
}
unsigned short *kmemsetw(unsigned short *dest, unsigned short val, int count) {
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

int kstrcmp (const char *s1, const char *s2) {
	while(*s1&&*s2){
        if(*s1-*s2){
            return *s1-*s2;
        }
        s1++;
        s2++;
    }
    if((*s1)&&!(*s2)){
        return *s1;
    }
    else if((*s2)&&!(*s1)) {
        return -(*s2);
    }

    return 0;
}

