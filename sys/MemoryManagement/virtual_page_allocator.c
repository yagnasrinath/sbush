/*
 * virtual_page_allocator.c
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/MemoryManagement/virtual_page_allocator.h>
#include<sys/sbunix.h>
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
	map_vir_to_phyaddr(curr_vir_addr,phy_addr, (USER_RW_FLAG|PAGE_PRESENT), TRUE);
	return curr_vir_addr;
}

// point the temporary page to null again
// Do not free the physical page
void free_temp_virtual_address(uint64_t virtual_addr) {
	uint64_t pt_vir_addr = get_pt_vir_addr(virtual_addr);
	(*((uint64_t *)pt_vir_addr))= 0;

}

uint64_t* vir_allocate_pages(int num_pages, uint64_t permissions) {
	//kprintf("Number of pages allocated  %d\n",num_pages );
	uint64_t start_addr = get_present_virtual_address();
	for(int i=0; i < num_pages; i++) {
		uint64_t phy_addr = allocate_phy_page();
		uint64_t virtual_addr = get_present_virtual_address();
		set_present_virtual_address(virtual_addr+PAGE_SIZE);
		map_vir_to_phyaddr(virtual_addr,phy_addr,permissions,TRUE);
	}
	return (uint64_t*) start_addr;
}


void ker_mmap(uint64_t start_vir_addr, uint64_t num_bytes, uint64_t perm) {
	uint64_t  current_vir_addr = get_present_virtual_address();
	set_present_virtual_address(PAGE_ALIGN(start_vir_addr));
	uint64_t temp = start_vir_addr + num_bytes-1;
	uint64_t num_of_pages = ((PAGE_ALIGN(temp) -(PAGE_ALIGN(start_vir_addr)))/PAGE_SIZE)+1;
	//uint64_t  end_virtual_address = start_vir_addr + num_bytes -1;
	//uint64_t num_of_pages = ((end_virtual_address - start_vir_addr)>>12) + 1;
	//kprintf("num of pages allocated  in kern_mmap%d \n", num_of_pages);
	vir_allocate_pages(num_of_pages,perm);
	set_present_virtual_address(current_vir_addr);
}






