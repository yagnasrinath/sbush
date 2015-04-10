/*
 * kmalloc.h
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_MEMORYMANAGEMENT_KMALLOC_H_
#define SBUSH_INCLUDE_SYS_MEMORYMANAGEMENT_KMALLOC_H_

#include<sys/defs.h>
#include<sys/MemoryManagement/virtual_page_allocator.h>
uint64_t* kmalloc(uint64_t size) ;

#endif /* SBUSH_INCLUDE_SYS_MEMORYMANAGEMENT_KMALLOC_H_ */
