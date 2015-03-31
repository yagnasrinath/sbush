/*
 * virtual_page_allocator.c
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/MemoryManagement/virtual_page_allocator.h>

/*
 * Normally holds the top of the kernel virtual address.
 *  i.e the address allocatable on the top of the kernel.
 *  But can me made to point to a temporary address and assign physical pages
 *  at that address.
 */

static uint64_t current_virtual_addr;

void set_present_virtual_address(uint64_t virtual_address) {
	current_virtual_addr = virtual_address;
}

uint64_t get_present_virtual_address() {
	return current_virtual_addr;
}

// get a temporary pages for things like memseting a physical page
uint64_t get_temp_virtual_address(uint64_t phy_addr) {
	uint64_t curr_vir_addr = get_present_virtual_address();
	map_vir_to_phyaddr(curr_vir_addr,phy_addr, (USER_RW_FLAG|PAGE_PRESENT));
	return current_virtual_addr;
}

// point the temporary page to null again
// Do not free the physical page
void free_temp_virtual_address(uint64_t virtual_addr) {
	uint64_t pt_vir_addr = get_pt_vir_addr(virtual_addr);
	(*((uint64_t *)pt_vir_addr))= 0;

}

uint64_t* vir_allocate_pages(int num_pages, uint64_t permissions) {
	uint64_t start_addr = get_present_virtual_address();
	for(int i=0; i < num_pages; i++) {
		uint64_t phy_addr = allocate_phy_page();
		uint64_t virtual_addr = get_present_virtual_address();
		set_present_virtual_address(virtual_addr+PAGE_SIZE);
		map_vir_to_phyaddr(virtual_addr,phy_addr,permissions);
	}
	return (uint64_t*) start_addr;
}

void free_page(uint64_t virtual_addr) {
	// returns  the virtual address for the pt entry
	uint64_t pt_entry= get_pt_vir_addr(virtual_addr);
	uint64_t phy_addr = (*((uint64_t *)pt_entry));
	phy_addr = phy_addr & PHY_PAGE_BITS;
	free_phy_page(phy_addr,TRUE);
	(*((uint64_t *)pt_entry))= 0;
	return;
}
