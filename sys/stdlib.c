/*
 * stdlib.c
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#include <sys/stdlib.h>
#include<sys/sbunix.h>
void halt(const char *reason, const char * const file, const char * const function, uint32_t line) {
	printf("%s %s:%d\n",file,function,line);
	if(reason && *reason)
		printf("%s\n",reason);
	printf("HALT");
	for(;;);
}
