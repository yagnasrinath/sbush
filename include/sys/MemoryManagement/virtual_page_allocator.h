/*
 * virtual_page_allocator.h
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_MEMORYMANAGEMENT_VIRTUAL_PAGE_ALLOCATOR_H_
#define SBUSH_INCLUDE_SYS_MEMORYMANAGEMENT_VIRTUAL_PAGE_ALLOCATOR_H_

#include<sys/defs.h>
#include<sys/MemoryManagement/phy_alloc.h>
#include<sys/MemoryManagement/MemoryManagementUtil.h>
void set_present_virtual_address(uint64_t virtual_address);
uint64_t get_present_virtual_address() ;

uint64_t get_temp_virtual_address(uint64_t phy_addr);
void free_temp_virtual_address(uint64_t virtual_addr);

uint64_t* vir_allocate_pages(int num_pages, uint64_t permissions);
void free_page(uint64_t virtual_addr);

void memset_physical_page(uint64_t phy_addr);
void ker_mmap(uint64_t start_vir_addr, uint64_t num_bytes, uint64_t perm) ;

#endif /* SBUSH_INCLUDE_SYS_MEMORYMANAGEMENT_VIRTUAL_PAGE_ALLOCATOR_H_ */
