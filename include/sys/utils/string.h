/*
 * string.h
 *
 *  Created on: Feb 27, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_UTILS_STRING_H_
#define SBUSH_INCLUDE_SYS_UTILS_STRING_H_

#include <sys/defs.h>
void *memcpy(void *dest, const void *src, int count);
void *memset(void *dest, char val, size_t count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
int strlen(const char *str);

#endif /* SBUSH_INCLUDE_SYS_UTILS_STRING_H_ */
