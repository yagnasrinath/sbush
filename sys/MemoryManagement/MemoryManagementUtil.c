#include<sys/MemoryManagement/MemoryManagementUtil.h>
static uint64_t kernel_cr3;
static uint64_t kernel_pml4;

void set_kernel_cr3(uint64_t kernel_phy_addr) {
	kernel_cr3 = kernel_phy_addr;
}

void set_kernel_pml4_entry(uint64_t kpml4) {
	kernel_pml4 = kpml4;
}

uint64_t get_pml4_vir_addr(uint64_t vir_addr){
	//self referencing virtual address for pml4e
	uint64_t pml4e_vir_addr = 0xFFFFFF7FBFDFE000UL;
	uint64_t pml4e_offset = vir_addr << 16 >> 55;
	//Entry pointing to the given virtual address in pml4e
	// shifiting 3 bits sicne each entry is  8 bytes
	uint64_t pml4e_pos = pml4e_vir_addr | (pml4e_offset << 3);
	return pml4e_pos;
}

uint64_t get_pdp_vir_addr(uint64_t vir_addr){
	//self referencing virtual address for pdp
	uint64_t pdp_vir_addr = 0xFFFFFF7FBFC00000UL;
	uint64_t pdp_offset = vir_addr << 16 >> 46;
	//Entry pointing to the given virtual address in pdp
	// shifiting 3 bits sicne each entry is  8 bytes
	uint64_t pdp_pos = pdp_vir_addr | (pdp_offset << 3);
	return pdp_pos;
}

uint64_t get_pd_vir_addr(uint64_t vir_addr){
	//self referencing virtual address for pd
	uint64_t pd_vir_addr = 0xFFFFFF7F80000000UL;
	uint64_t pd_offset = vir_addr << 16 >> 37;
	//Entry pointing to the given virtual address in pd
	// shifiting 3 bits sicne each entry is  8 bytes
	uint64_t pd_pos = pd_vir_addr | (pd_offset << 3);
	return pd_pos;
}

uint64_t get_pt_vir_addr(uint64_t vir_addr){
	//self referencing virtual address for pt
	uint64_t pt_vir_addr = 0xFFFFFF0000000000UL;
	uint64_t pt_offset = vir_addr << 16 >> 28;
	//Entry pointing to the given virtual address in pt
	// shifiting 3 bits sicne each entry is  8 bytes
	uint64_t pt_pos = pt_vir_addr | (pt_offset << 3);
	return pt_pos;
}


void map_vir_to_phyaddr(uint64_t viraddr, uint64_t phyaddr, uint64_t flags){
	uint64_t *pml4_entry, *pdp_entry, *pd_entry, *pt_entry;

	pml4_entry = (uint64_t *)get_pml4_vir_addr(viraddr);


	if(!IS_PAGE_PRESENT(*pml4_entry)){
		*pml4_entry = allocate_phy_page() |USER_RW_FLAG | PAGE_PRESENT;
	}

	pdp_entry = (uint64_t *)get_pdp_vir_addr(viraddr);
	if(!IS_PAGE_PRESENT(*pdp_entry)){
		*pdp_entry = allocate_phy_page() | USER_RW_FLAG | PAGE_PRESENT;
	}

	pd_entry = (uint64_t *)get_pd_vir_addr(viraddr);
	if(!IS_PAGE_PRESENT(*pd_entry)){
		*pd_entry = allocate_phy_page() | USER_RW_FLAG | PAGE_PRESENT;
	}

	pt_entry = (uint64_t *)get_pt_vir_addr(viraddr);
	if(IS_PAGE_PRESENT(*pt_entry)){
		free_phy_page(phyaddr, TRUE);
	}
	*pt_entry = phyaddr | flags;
 }

uint64_t get_new_pml4_t() {
	uint64_t phy_page = allocate_phy_page();
	uint64_t vaddr = get_present_virtual_address();
	set_present_virtual_address(get_present_virtual_address() + PAGE_SIZE);
	map_vir_to_phyaddr(vaddr,phy_page, PAGE_PRESENT|USER_RW_FLAG);
	uint64_t *vir_addr = (uint64_t *)vaddr;
	//self referencing
	vir_addr[510] = phy_page|PAGE_PRESENT|USER_RW_FLAG;
	vir_addr[511] = kernel_pml4;
	return phy_page;
}


uint64_t get_pte_entry(uint64_t vir_addr){
	return *(uint64_t*)(get_pt_vir_addr(vir_addr));
}

