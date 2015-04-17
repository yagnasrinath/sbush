/*
 * string.h
 *
 *  Created on: Feb 27, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_UTILS_KSTRING_H_
#define SBUSH_INCLUDE_SYS_UTILS_KSTRING_H_

#include <sys/defs.h>
#include <sys/sbunix.h>
void *kmemcpy(void *dest, const void *src, int count);
void *kmemset(void *dest, char val, size_t count);
char *kstrcpy ( char *__restrict dest, const char *__restrict src) ;
char *kstrncpy (char *__restrict dest, const char *__restrict src, const size_t n);
unsigned short *kmemsetw(unsigned short *dest, unsigned short val, int count);
int kstrlen(const char *str);
int kstrcmp (const char *s1, const char *s2) ;
int32_t atoi(char *p);
#endif /* SBUSH_INCLUDE_SYS_UTILS_KSTRING_H_ */
