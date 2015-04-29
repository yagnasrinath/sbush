/*
 * string.c
 *
 *  Created on: Feb 27, 2015
 *      Author: ravichandrasadineni
 */
#include "../../include/sys/utils/kstring.h"

void *kmemcpy(void *dest, void *source, uint64_t count){
	char *des = (char *) dest;
	const char* src = (char *)source;

	for(;count;count--){
		*des++ =*src++;
	}

	return dest;
}


/*void *kmemcpy(void *destination, void *source, uint64_t num)
{
    uint8_t *dest = (uint8_t *)destination;
    uint8_t *src = (uint8_t *)source;

    while(num--) {
        *dest++ = *src++;
    }

    return destination;
}*/

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

int kstrlen(const char *str) {
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


int32_t atoi(char *p)
{
	int sign =1;
	if (p[0] == '-' || p[0] == '+') {
		if(p[0] == '-' )
			sign = -1;
		p++;
	}
	int i=0, output = 0;
	while (p[i]!= '\0') {
		if (p[i] >= 48 && p[i] <= 57) {
			output = 10*output+p[i]-'0';
			i++;
		} else {
			return 0;
		}

	}
	return output*sign;
}


char *kstrcpy ( char *__restrict dest, const char *__restrict src) {
	char* orgdest = dest;
	while((*dest = *src)){
		dest++;
		src++;
	}
	return orgdest;
}


char *kstrncpy (char *__restrict dest, const char *__restrict src, const size_t n) {
	char* orgdest =dest;
	int i=0;
	while((i++ < n)&&(*dest++ = *src++));

	if(i<n) {
		dest[i] = '\0';
	}
	return orgdest;
}

static char * strtok_r(char *s, const char *delim, char **last)
{
    char *spanp;
    int c, sc;
    char *tok;


    if (s == NULL && (s = *last) == NULL)
        return (NULL);

    /*
     *   * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     *       */
cont:
    c = *s++;
    for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
        if (c == sc)
            goto cont;
    }

    if (c == 0) {       /* no non-delimiter characters */
        *last = NULL;
        return (NULL);
    }
    tok = s - 1;

    /*
     *   * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     *       * Note that delim must have one NUL; we stop if we see that, too.
     *           */
    for (;;) {
        c = *s++;
        spanp = (char *)delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *last = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}


char * kstrtok(char *s, const char *delim)
{
    static char *last;

    return strtok_r(s, delim, &last);
}

