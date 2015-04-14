#ifndef __SBUNIX_H
#define __SBUNIX_H

#include <sys/defs.h>

void kprintf(const char *fmt, ...);
static inline void panic(char * reason) {
	kprintf("\n FATAL ERROR %s \n",reason);
	while(1);
}

#endif
