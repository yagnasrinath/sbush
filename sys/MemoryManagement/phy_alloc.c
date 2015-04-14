/*
 * phy_alloc.c
 *
 *  Created on: Mar 12, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/MemoryManagement/phy_alloc.h>
#include<sys/MemoryManagement/virtual_page_allocator.h>
#include <sys/sbunix.h>
#include "../../include/sys/utils/kstring.h"
static char mem_bitmap[(NUM_PAGES/8)];
static char ref_count[NUM_PAGES];
int num_of_phy_blocks = 0;

static void mark_page_free(uint64_t pageNum) {
	uint64_t pos = pageNum/8;
	uint64_t offset = pageNum%8;
	uint64_t orval = 1 <<offset;
	mem_bitmap[pos] |=orval;
}

static void mark_page_used(uint64_t pageNum) {
	uint64_t pos = pageNum/8;
	uint64_t offset = pageNum%8;
	uint64_t orval = 1 <<offset;
	mem_bitmap[pos] &= ~(orval);
}

 void dec_phy_page_ref_count(uint64_t pageNum) {
	ref_count[pageNum]--;
}

 void inc_phy_page_ref_count(uint64_t pageNum) {
	ref_count[pageNum]++;
}

 int get_phy_page_ref_count(uint64_t pageNum) {
	return ref_count[pageNum];
}


// returning false
//1. if the length is zero
//2.  if the memory cant be allocated
//3. if base points to 0 ??? why ????
//4. IF video buffer return FALSE
//5. If kernel space return FALSE
//6. If a null pointer return straight away.

static BOOL is_allocatable_memory(struct smap_t* smap,void* phy_base, void* phy_free) {

	// If a NULL pointer. Then return straight away
	if(smap->base == 0X0) {
			return FALSE;
	}
	// Increasing the size of the phy_free by 1 MB to control the growing kernel stack
	uint64_t updated_phy_free = (uint64_t)phy_free + 1024*1024;
	if (smap->type != 1  || smap->length == 0) {
		return FALSE;
	}
	// Prevent corrupting kernel space
	if ((smap->base >=(uint64_t)phy_base) && ((smap->base + smap->length)  <=updated_phy_free)) {
		return FALSE;
	}

	// Prevent corrupting video buffer
	if (smap->base >= 0xB8000 && smap->base <= 0xBC096) {
		return FALSE;
	}
	// Do not go below physical base. It is a hole there
/*	if (smap->base < (uint64_t)phy_base) {
		return FALSE;
	}*/

	return TRUE;
}

void get_4k_aligned(uint64_t *addr,uint64_t *length)
{
    if(((*addr)&PAGE_OFFSET)==0)
    {
        return;//address aready 4k aligned
    }
    //first get the aligned block and return one higher
    //than the currently algined block(add 4k)
    uint64_t residue = *addr&PAGE_OFFSET;
    *addr = ((*addr)&ALIGN_4K)+0x0000000000001000;
    // decrementing length as we reduced the space
    if(((*length)%PAGE_SIZE)==0)
    {
        *length = *length-PAGE_SIZE;
    }
    else
    {
        *length = *length - residue;
    }
    return;
}


void init_phy_memory(struct smap_t* smap, int smap_num, void* phy_base, void* phy_free ) {
	// marking all pages used initially
	kmemset(mem_bitmap, 0x00, sizeof mem_bitmap);
	uint64_t updated_phy_free = (uint64_t)phy_free + 1024*1024;

	for (int i=0; i<smap_num; i++) {
		// marking allocatable pages available
		if(is_allocatable_memory(smap,phy_base,phy_free)) {
			uint64_t addr = smap->base;
			uint64_t length = smap->length;
                        get_4k_aligned(&addr,&length);
			for(;length>=PAGE_SIZE; length-=PAGE_SIZE, addr+=PAGE_SIZE) {
				if((addr >= (uint64_t)phy_base) && (addr <= (uint64_t)updated_phy_free) ) {
					continue;
				}
				mark_page_free(mm_phy_to_page(addr));
			}
		}
		smap++;
	}

}


void free_phy_page(uint64_t page_addr,BOOL zeroPage) {
	int page_num = page_addr/PAGE_SIZE;
	if(page_num == 0)
		return;
	dec_phy_page_ref_count(page_num);

	if(get_phy_page_ref_count(page_num) ==0) {
		if(zeroPage) {
			uint64_t vir_addr = get_temp_virtual_address(page_addr);
			kmemset((uint64_t *)vir_addr, 0,PAGE_SIZE);
			free_temp_virtual_address(vir_addr);
		}
		mark_page_free(page_num);
	}
	return;
}


uint64_t allocate_phy_page() {
	num_of_phy_blocks++;
	for(uint64_t i=0; i < sizeof mem_bitmap; i++ ) {
			// if all pages are not used then try to find a free page
			if(mem_bitmap[i]!= 0) {
				char curr_bitMap =mem_bitmap[i] ;

				for(int curr_page = 0;curr_page < PAGES_PER_GROUP; curr_page++) {
					if(curr_bitMap&(1<<curr_page )) {
							uint64_t free_page =  i*PAGES_PER_GROUP + curr_page;
							mark_page_used(free_page);
							inc_phy_page_ref_count(free_page);
							return free_page*PAGE_SIZE;
					}

				}
			}
	}
	kprintf("Number of physical blocks assigned %d \n", num_of_phy_blocks);
	panic("PHYSICAL MEMORY NOT AVIALABLE\n");
	return 0;
}
