#include<sys/MemoryManagement/MemoryManagementUtil.h>
#include<sys/system.h>
#include<sys/sbunix.h>
#include<sys/utils/kstring.h>
#include<sys/defs.h>
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

uint64_t get_pdp_beg(uint64_t vir_addr){
	//self referencing virtual address for pdp
	uint64_t pdp_vir_addr = 0xFFFFFF7FBFC00000UL;
	uint64_t pdp_offset = vir_addr << 16 >> 55<<9;
	//Entry pointing to the given virtual address in pdp
	// shifiting 3 bits sicne each entry is  8 bytes
	uint64_t pdp_pos = pdp_vir_addr | (pdp_offset << 3);
	return pdp_pos;
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

uint64_t get_pd_beg(uint64_t vir_addr){
	//self referencing virtual address for pd
	uint64_t pd_vir_addr = 0xFFFFFF7F80000000UL;
	uint64_t pd_offset = vir_addr << 16 >> 46<<9;
	//Entry pointing to the given virtual address in pd
	// shifiting 3 bits sicne each entry is  8 bytes
	uint64_t pd_pos = pd_vir_addr | (pd_offset << 3);
	return pd_pos;
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


uint64_t get_pt_beg(uint64_t vir_addr){
	//self referencing virtual address for pt
	uint64_t pt_vir_addr = 0xFFFFFF0000000000UL;
	uint64_t pt_offset = vir_addr << 16 >> 37<<9;
	//Entry pointing to the given virtual address in pt
	// shifiting 3 bits sicne each entry is  8 bytes
	uint64_t pt_pos = pt_vir_addr | (pt_offset << 3);
	return pt_pos;
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

uint64_t get_page_beg(uint64_t vir_addr){
	//self referencing virtual address for pt
	//uint64_t pt_vir_addr = 0xFFFF0000000000UL;
	uint64_t pg_offset = vir_addr >> 12 << 12;
	//Entry pointing to the given virtual address in pt
	// shifiting 3 bits sicne each entry is  8 bytes

	return pg_offset;
}





BOOL is_page_present_for_virtual_addr(uint64_t viraddr) {
	uint64_t *pml4_entry, *pdp_entry, *pd_entry, *pt_entry;

		pml4_entry = (uint64_t *)get_pml4_vir_addr(viraddr);


		if(!IS_PAGE_PRESENT(*pml4_entry)){
			return FALSE;
		}

		pdp_entry = (uint64_t *)get_pdp_vir_addr(viraddr);
		if(!IS_PAGE_PRESENT(*pdp_entry)){
			return FALSE;
		}

		pd_entry = (uint64_t *)get_pd_vir_addr(viraddr);
		if(!IS_PAGE_PRESENT(*pd_entry)){
			return FALSE;
		}

		pt_entry = (uint64_t *)get_pt_vir_addr(viraddr);
		if(!IS_PAGE_PRESENT(*pt_entry)){
			return FALSE;
		}
		return TRUE;


}


void map_vir_to_phyaddr(uint64_t viraddr, uint64_t phyaddr, uint64_t flags, BOOL zeroFlag){
	volatile uint64_t *pml4_entry, *pdp_entry, *pd_entry, *pt_entry;

	pml4_entry = (uint64_t *)get_pml4_vir_addr(viraddr);


	if(!IS_PAGE_PRESENT(*pml4_entry)){
		uint64_t pml4 = allocate_phy_page();
		//kprintf("pml4t->%p\n",pml4);

		*pml4_entry = pml4 |USER_RW_FLAG | PAGE_PRESENT;
		uint64_t* pdp_beg = (uint64_t *)get_pdp_beg(viraddr);
		kmemset(pdp_beg, '\0',PAGE_SIZE);
	}

	pdp_entry = (uint64_t *)get_pdp_vir_addr(viraddr);
	if(!IS_PAGE_PRESENT(*pdp_entry)){
		uint64_t pdp = allocate_phy_page();
		//kprintf("pdp->%p\n",pdp);

		*pdp_entry = pdp | USER_RW_FLAG | PAGE_PRESENT;
		uint64_t* pd_beg = (uint64_t *)get_pd_beg(viraddr);
		kmemset(pd_beg, '\0',PAGE_SIZE);
	}

	pd_entry = (uint64_t *)get_pd_vir_addr(viraddr);
	if(!IS_PAGE_PRESENT(*pd_entry)){
		uint64_t pd = allocate_phy_page();
		//kprintf("pd->%p\n",pd);

		*pd_entry = pd | USER_RW_FLAG | PAGE_PRESENT;
		uint64_t* pt_beg = (uint64_t *)get_pt_beg(viraddr);
		kmemset(pt_beg, '\0',PAGE_SIZE);
	}

	pt_entry = (uint64_t *)get_pt_vir_addr(viraddr);
	if(IS_PAGE_PRESENT(*pt_entry)){

		kprintf(" page present for %p \n", viraddr);
		panic("page mapped where it should not be ");
		free_phy_page(*pt_entry, TRUE);
	}
	//kprintf("phy addr for mapping is %p \n ", phyaddr);
	*pt_entry = phyaddr | flags;
	if(zeroFlag) {
		//kprintf("Zero flag passed  are %p\n", viraddr);
		uint64_t* pg_beg = (uint64_t *)get_page_beg(viraddr);
		kmemset(pg_beg,'\0', PAGE_SIZE);
	}
	//kmemset(pt_entry, '\0',PAGE_SIZE);
	//kprintf("phyaddr->%p\n",phyaddr);

}

uint64_t get_new_pml4_t() {
	uint64_t phy_page = allocate_phy_page();
	uint64_t vaddr = get_present_virtual_address();
	set_present_virtual_address(get_present_virtual_address() + PAGE_SIZE);
	map_vir_to_phyaddr(vaddr,phy_page, PAGE_PRESENT|USER_RW_FLAG,TRUE);
	uint64_t *vir_addr = (uint64_t *)vaddr;
	//self referencing
	vir_addr[510] = phy_page|PAGE_PRESENT|USER_RW_FLAG;
	vir_addr[511] = kernel_pml4;
	return phy_page;
}

void free_pagetables(){

	uint64_t pml4e_vir_addr = 0xFFFFFF7FBFDFE000UL;
	uint64_t pdp_vir_addr = 0xFFFFFF7FBFC00000UL;
	uint64_t pd_vir_addr = 0xFFFFFF7F80000000UL;
	uint64_t pt_vir_addr = 0xFFFFFF0000000000UL;
	uint64_t *pt_addr = NULL;
	uint64_t *pd_addr = NULL;
	uint64_t *pg_addr = NULL;
	uint64_t *pdp_addr  = NULL;
	uint64_t i=0;
	uint64_t j=0;
	uint64_t k=0;
	uint64_t l=0;
	//uint64_t norm_addr = 0xFFFF000000000000UL;
	for(i=0;i<=509;i++){
		pdp_addr = (uint64_t*)(pml4e_vir_addr | (i << 3));
		if(IS_PAGE_PRESENT(*pdp_addr)){
			for( j=0; j <= 511; j++){
				pd_addr = (uint64_t*)(pdp_vir_addr | (i<<12) | (j << 3));
				if(IS_PAGE_PRESENT(*pd_addr)){
					for(k=0; k <= 511; k++){
						pt_addr = (uint64_t*)(pd_vir_addr | (i<<21) | (j<<12) | (k << 3));
						if(IS_PAGE_PRESENT(*pt_addr)){
							for(l=0; l <= 511; l++){
								pg_addr = (uint64_t*)(pt_vir_addr | (i<<30) | (j<<21) | (k<<12) | (l << 3));
								if(IS_PAGE_PRESENT(*pg_addr)){
									//uint64_t* curr_vir_addr = (uint64_t*)(norm_addr | (i<<39) | (j<<30) | (k<<21) | (l << 12));
									//	kmemset(curr_vir_addr,'\0', PAGE_SIZE);
									free_phy_page(*pg_addr, TRUE);
									invlpg(pg_addr);
									*pg_addr = 0;
								}

							}

							free_phy_page(*pt_addr, TRUE);
							invlpg(pt_addr);
							*pt_addr = 0;
						}
					}
					free_phy_page(*pd_addr, TRUE);
					invlpg(pd_addr);
					*pd_addr = 0;
				}
			}
			free_phy_page(*pdp_addr, TRUE);
			invlpg(pdp_addr);
			*pdp_addr = 0;
		}
	}
	//kprintf("page tables freed\n");
	//kprintf("\nkerncr3 is %p \n", kernel_cr3);
	set_cr3(kernel_cr3);
	//Cleaning Process

}


