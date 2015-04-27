/*
 * phy_alloc.h
 *
 *  Created on: Mar 12, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_SYS_MEMOYMANAGEMENT_PHY_ALLOC_H_
#define SBUSH_SYS_MEMOYMANAGEMENT_PHY_ALLOC_H_
#include<sys/defs.h>
struct smap_t {
	uint64_t base, length;
	uint32_t type;
}__attribute__((packed)) *smap;
#define NUM_PAGES 32768
#define PAGE_SIZE 4096
#define PAGES_PER_GROUP 8
#define ALIGN_4K 0xFFFFFFFFFFFFF000
#define PAGE_OFFSET 0x0000000000000FFF

void init_phy_memory(struct smap_t* smap, int smap_num, void* phy_base,
		void* phy_free);

void free_phy_page(uint64_t page_addr, BOOL zeroPage);

uint64_t allocate_phy_page();
void memset_phy_pages();

static inline uint64_t mm_phy_to_page(uint64_t phy) {

	return phy / PAGE_SIZE;
}
void inc_phy_page_ref_count(uint64_t pageNum);
void dec_phy_page_ref_count(uint64_t pageNum);
uint64_t get_phy_page_ref_count(uint64_t pageNum);

#endif /* SBUSH_SYS_MEMOYMANAGEMENT_PHY_ALLOC_H_ */
