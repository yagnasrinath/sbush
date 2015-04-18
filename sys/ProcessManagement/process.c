/*
 * process.c
 *
 *  Created on: Apr 17, 2015
 *      Author: ravichandrasadineni
 */


#include<sys/ProcessManagement/process.h>
#include<sys/ProcessManagement/process_scheduler.h>
#include<sys/system.h>
#include<sys/sbunix.h>
#include<sys/ProcessManagement/process_helper.h>
#include<sys/MemoryManagement/virtual_page_allocator.h>
#include<sys/MemoryManagement/MemoryManagementUtil.h>
#include<sys/MemoryManagement/phy_alloc.h>
#include<sys/utils/kstring.h>

task_struct* copy_task_struct(task_struct* parent_task_struct){
	if(parent_task_struct == NULL){
		panic("process.c: Parent Struct NULL\n");
	}
	task_struct* child_task_struct = create_new_task(TRUE);
	uint64_t child_pml4 = child_task_struct->virtual_addr_space->pml4_t;
	uint64_t parent_pml4 = parent_task_struct->virtual_addr_space->pml4_t;
	kmemcpy(child_task_struct->virtual_addr_space, parent_task_struct->virtual_addr_space, sizeof(mem_struct));
	child_task_struct->virtual_addr_space->pml4_t = child_pml4;
	child_task_struct->virtual_addr_space->vmaList = NULL;

	//copying from parent to child
	vma_struct* parent_vma = parent_task_struct->virtual_addr_space->vmaList;
	vma_struct* child_vma = NULL;
	vma_struct* child_vmalist_start = NULL;

	while(parent_vma != NULL){
		vma_struct* new_child_vma = create_new_vma(parent_vma->file_descp, parent_vma->vm_area_start, parent_vma->vm_area_end, parent_vma->vma_type,parent_vma->vma_perm);
		if(child_vma == NULL){
			child_vma = new_child_vma;
			child_vmalist_start = child_vma;
		}
		else{
			child_vma->next = new_child_vma;
			child_vma = child_vma->next;
		}
		if(parent_vma->vma_type == STACK){
			uint64_t vma_start = PAGE_ALIGN(parent_vma->vm_area_end) - PAGE_SIZE;
			uint64_t curr_kern_vaddr = get_present_virtual_address();
			while(vma_start >= parent_vma->vm_area_start){
				uint64_t* pte_entry = (uint64_t *)get_pt_vir_addr(vma_start);
				if(((*pte_entry) & PAGE_PRESENT)){
					break;
				}
				uint64_t new_phy_page = allocate_phy_page();
				map_vir_to_phyaddr(curr_kern_vaddr, new_phy_page, USER_RW_FLAG);
				kmemcpy((uint64_t*)curr_kern_vaddr, (uint64_t*)vma_start, PAGE_SIZE);
				set_cr3(child_pml4);
				map_vir_to_phyaddr(vma_start, new_phy_page, USER_RW_FLAG);
				set_cr3(parent_pml4);
				uint64_t *pte_entry_kern_vir_addr = (uint64_t *)get_pt_vir_addr(curr_kern_vaddr);
				*pte_entry_kern_vir_addr = 0;
				vma_start -= PAGE_SIZE;
			}
		}
		else{
			uint64_t vma_start = PAGE_ALIGN(parent_vma->vm_area_start);
			uint64_t vma_end = PAGE_ALIGN(parent_vma->vm_area_end);
			while(vma_start <= vma_end){
				uint64_t* parent_pte_entry = (uint64_t*)get_pt_vir_addr(vma_start);
				if((*parent_pte_entry) & PAGE_PRESENT){
					if((*parent_pte_entry) & RW_FLAG){
						new_child_vma->vma_perm = COPY_ON_WRITE;
						parent_vma->vma_perm = COPY_ON_WRITE;
					}
					*parent_pte_entry = (*parent_pte_entry) & ~RW_FLAG;
					uint64_t child_pte_entry = (*parent_pte_entry) & UNSET_FLAGS;
					uint64_t child_pte_flags = (*parent_pte_entry) & FLAGS;
					set_cr3(child_pml4);
					map_vir_to_phyaddr(vma_start,child_pte_entry,child_pte_flags);
					set_cr3(parent_pml4);
					inc_phy_page_ref_count(*parent_pte_entry);
				}
				vma_start += PAGE_SIZE;
			}

		}
		child_task_struct->virtual_addr_space->vmaList = child_vmalist_start;
		parent_vma = parent_vma->next;
	}
	kstrcpy(child_task_struct->task_name, parent_task_struct->task_name);
	child_task_struct->ppid = parent_task_struct->pid;
	child_task_struct->next = NULL;
	child_task_struct->parent = parent_task_struct;
	return child_task_struct;
}