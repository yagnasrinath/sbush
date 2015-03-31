/*
 * MemoryManagementUtil.h
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_MEMORYMANAGEMENT_MEMORYMANAGEMENTUTIL_H_
#define SBUSH_INCLUDE_SYS_MEMORYMANAGEMENT_MEMORYMANAGEMENTUTIL_H_

#include<sys/defs.h>
#include<sys/MemoryManagement/MemoryManagementUtil.h>
#include<sys/MemoryManagement/phy_alloc.h>
#define KERNEL_RW_FLAG 3UL
#define USER_RW_FLAG 6UL
#define PAGE_PRESENT 1UL
#define PHY_PAGE_BITS 0x000FFFFFFFFFF000UL


#define IS_PAGE_PRESENT(page_entry) ((page_entry) & 0X1)

uint64_t get_pml4_vir_addr(uint64_t vir_addr);
uint64_t get_pdp_vir_addr(uint64_t vir_addr);
uint64_t get_pd_vir_addr(uint64_t vir_addr);
uint64_t get_pt_vir_addr(uint64_t vir_addr);
void map_vir_to_phyaddr(uint64_t viraddr, uint64_t phyaddr, uint64_t flags);
#endif /* SBUSH_INCLUDE_SYS_MEMORYMANAGEMENT_MEMORYMANAGEMENTUTIL_H_ */
