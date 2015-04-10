/*
 * kmalloc.c
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */


#include<sys/MemoryManagement/kmalloc.h>
#include <sys/sbunix.h>
#include "../../include/sys/utils/kstring.h"
static uint64_t current_addr =0;
static uint64_t current_avail_size =0;
#define BLOCK_ALIGN 16

uint64_t* kmalloc(uint64_t size) {
	uint64_t* ret_addr;
	if(size%BLOCK_ALIGN ) {
		size = (size/BLOCK_ALIGN)*BLOCK_ALIGN;
		size  += BLOCK_ALIGN;
	}
	if(size > current_avail_size) {
		int numOfPages = size/PAGE_SIZE;
		if(size%PAGE_SIZE) {
			numOfPages +=1;
		}
		ret_addr = vir_allocate_pages(numOfPages, KERNEL_RW_FLAG|PAGE_PRESENT);
		current_avail_size = numOfPages*PAGE_SIZE - size;
		current_addr = (uint64_t)ret_addr + size;
	}
	else {
		current_avail_size -= size;
		ret_addr = (uint64_t *)current_addr;
		current_addr += size;
	}
	return ret_addr;
}

