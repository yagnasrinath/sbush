#ifndef _STRING_H
#define _STRING_H   1
#include<stdlib.h>
extern char *strcpy (char *__restrict __dest, const char *__restrict __src);
         /* Copy no more than N characters of SRC to DEST.  */
extern char *strncpy (char *__restrict __dest,
                              const char *__restrict __src, size_t __n);

extern int strcmp (const char *__s1, const char *__s2);

extern int strncmp (const char *__s1, const char *__s2, size_t __n);

extern char *strcat (char *__restrict __dest, const char *__restrict __src);
extern char *strncat (char *__restrict __dest, const char *__restrict __src,
                      size_t __n);
extern size_t strlen (const char *__s);
void *memset(void* input,int c,size_t size);
char *strstr(char *haystack, char *needle);
#endif /* string.h  */

