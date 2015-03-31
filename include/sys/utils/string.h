/*
 * string.h
 *
 *  Created on: Feb 27, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_UTILS_STRING_H_
#define SBUSH_INCLUDE_SYS_UTILS_STRING_H_

#include <sys/defs.h>
#include <sys/sbunix.h>
void *memcpy(void *dest, const void *src, int count);
void *memset(void *dest, char val, size_t count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
int strlen(const char *str);
int strcmp (const char *s1, const char *s2) ;
#endif /* SBUSH_INCLUDE_SYS_UTILS_STRING_H_ */
