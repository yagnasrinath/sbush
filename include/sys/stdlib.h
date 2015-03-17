/*
 * stdlib.h
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_STDLIB_H_
#define SBUSH_INCLUDE_SYS_STDLIB_H_
#include<sys/defs.h>
void halt(const char *reason, const char * const file, const char * const function, uint32_t line);

#define HALT(reason) halt(reason,__FILE__,__FUNCTION__,__LINE__)

#endif /* SBUSH_INCLUDE_SYS_STDLIB_H_ */
