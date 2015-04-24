/*
 * isr_handler.c
 *
 *  Created on: Apr 15, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/defs.h>
#include<sys/utils/kstring.h>
#include<sys/idt.h>
#include<sys/sbunix.h>
#include<sys/ProcessManagement/process_scheduler.h>
#include<sys/MemoryManagement/virtual_page_allocator.h>
#include<sys/SyscallManagement/syscall.h>

static void print_regiters(struct isr_nrm_regs regs) {
	kprintf("r15:%p",regs.r15);
	kprintf(";r14:%p",regs.r14);
	kprintf(";r13:%p",regs.r13);
	kprintf(";r12:%p",regs.r12);
	kprintf(";r11:%p",regs.r11);
	kprintf(";r10:%p",regs.r10);
	kprintf(";r9:%p",regs.r9);
	kprintf(";r8:%p",regs.r8);
	kprintf(";rsi:%p",regs.rsi);
	kprintf(";rbp:%p",regs.rbp);
	kprintf(";rdx:%p",regs.rdx);
	kprintf(";rcx:%p",regs.rcx);
	kprintf(";rbx:%p",regs.rbx);
	kprintf(";rax:%p",regs.rax);
	kprintf(";rdi:%p",regs.rdi);
	kprintf(";int_no:%p",regs.interrupt);
	kprintf(";err_no:%p",regs.error);
	kprintf(";rip:%p",regs.rip);
	kprintf(";cs:%p",regs.cs);
	kprintf(";rflags:%p",regs.rflags);
	kprintf(";rsp:%p",regs.rsp);
	kprintf(";ss:%p",regs.ss);
}

void divide_by_zero_handler(struct isr_nrm_regs regs) {
	print_regiters(regs);
	panic("Divide By Zero Error ");
}

void tss_fault_handler(struct isr_nrm_regs regs) {
	panic("TSS FAULt error");
}

void gpf_handler(struct isr_nrm_regs regs) {

	uint64_t lcr2 = 3;
	__asm__ __volatile__ ("movq %%cr2, %0;" : "=r"(lcr2));
	uint64_t lcr3 =3;
	__asm__ __volatile__ ("movq %%cr3, %0;" : "=r"(lcr3));
	uint64_t lrsp =3;
	__asm__ __volatile__ ("movq %%rsp, %0;" : "=r"(lrsp));
	kprintf("protection  handler cr3 %p \n",lcr3);
	kprintf("protection  handler cr2 %p \n",lcr2);
	kprintf("protection  handler errno %d \n",regs.error);
	kprintf("protection_fault_handler rsp %p \n",lrsp);
	print_regiters(regs);
	panic("Protection fault handler");
}

void page_fault_handler(struct isr_nrm_regs regs) {
	uint64_t lcr2 = 3;
	__asm__ __volatile__ ("movq %%cr2, %0;" : "=r"(lcr2));
	uint64_t lcr3 =3;
	__asm__ __volatile__ ("movq %%cr3, %0;" : "=r"(lcr3));
	uint64_t lrsp =3;
	__asm__ __volatile__ ("movq %%rsp, %0;" : "=r"(lrsp));
	kprintf("\n page_fault_handler cr3 %p \n",lcr3);
	kprintf("page_fault_handler cr2 %p \n",lcr2);
	kprintf("page_fault_handler rsp %p \n",lrsp);
	kprintf("page fault  handler errno %d \n",regs.error);
	uint64_t fault_addr = lcr2;
	if(lcr2 >= USR_STK_TOP) {
		panic("PAGE FAULT IN KERNEL\n");
	}
	if(regs.error & 0x1) { // PAGE PRESENT
		task_struct* curr_task = get_curr_task();
		vma_struct* curr_vmaList = curr_task->virtual_addr_space->vmaList;
		if(curr_vmaList  == NULL) {
			kprintf("Page present : Process name %s\n",curr_task->task_name);
			panic("No vma list for the process");
		}
		vma_struct* curr_vma = curr_vmaList;
		while(curr_vma != NULL) {
			uint64_t curr_start = curr_vma->vm_area_start;
			uint64_t curr_end = curr_vma->vm_area_end;
			if((fault_addr  >= curr_start) && (fault_addr <= curr_end)) {
				if(curr_vma->vma_perm == COPY_ON_WRITE) {
					uint64_t* pte_entry = (uint64_t *)get_pt_vir_addr(fault_addr);
					int curr_ref_count = get_phy_page_ref_count((*pte_entry)/PAGE_SIZE);
					//kprintf("page fault handler: ref count is %d\n", curr_ref_count);
					if(curr_ref_count <= 0) {
						panic("something wrong reference count is  less than or equal to zero");
					}
					if(curr_ref_count  == 1) {
						//kprintf("ref conut is 1\n");
						* pte_entry  = * pte_entry | USER_RW_FLAG;
						set_cr3(lcr3);
					}
					else if(curr_ref_count  < 1) {
						kprintf("page fault for page with reference count%d", curr_ref_count);
						panic("");
					}
					else {
						//kprintf("entered else \n");
						//kprintf("pte entry is %d \n",(* pte_entry)/PAGE_SIZE);
						uint64_t new_phy_page = allocate_phy_page();
						uint64_t curr_kern_vaddr = get_present_virtual_address();
						map_vir_to_phyaddr(curr_kern_vaddr, new_phy_page, USER_RW_FLAG| PAGE_PRESENT);
						kmemcpy((uint64_t*)curr_kern_vaddr, (uint64_t*)PAGE_ALIGN(fault_addr), PAGE_SIZE);
						map_vir_to_phyaddr(fault_addr, new_phy_page, USER_RW_FLAG|PAGE_PRESENT);
						uint64_t *pte_entry_kern_vir_addr = (uint64_t *)get_pt_vir_addr(curr_kern_vaddr);
						*pte_entry_kern_vir_addr = 0;
						//ker_mmap(fault_addr, PAGE_SIZE, PAGE_PRESENT | USER_RW_FLAG);
						//kprintf("came out of else \n");
					}
					break;
				}

			}
			curr_vma = curr_vma->next;
		}
		if(curr_vma == NULL ) {
			panic("PAGE FAULT IN Not Handled when PAGE is PRESENT AND NOT COW\n");
		}
	}
	else {
		kprintf("entered the else case in page fault  %p\n",fault_addr );
		task_struct* curr_task = get_curr_task();
		vma_struct* curr_vmaList = curr_task->virtual_addr_space->vmaList;
		if(curr_vmaList  == NULL) {
			kprintf("Page not present : Process name %s\n",curr_task->task_name);
			panic("No vma list for the process");
		}
		vma_struct* curr_vma = curr_vmaList;
		fault_addr = PAGE_ALIGN(fault_addr);
		//kprintf("Fault address is %p\n",fault_addr );
		while(curr_vma != NULL) {
			uint64_t curr_start = curr_vma->vm_area_start;
			uint64_t curr_end = curr_vma->vm_area_end;
			//kprintf("VMA start %p\n",curr_start );
			//kprintf("VMA end %p\n",curr_end );

			if((fault_addr  >= curr_start) && (fault_addr <= curr_end)) {
				ker_mmap(fault_addr, PAGE_SIZE, PAGE_PRESENT | USER_RW_FLAG);
				//kprintf("Memory allocated for  %p\n",fault_addr );
				break;
			}
			curr_vma = curr_vma->next;
		}
		if(curr_vma == NULL) {
			print_regiters(regs);
			// should exit process and throw segmentation fault
			panic("Process accessed unassigned memory \n");
		}
	}

}


void isr_handler(struct isr_nrm_regs regs)
{
	switch (regs.interrupt) {
	case 0:
		divide_by_zero_handler(regs);
		break;
	case 10:
		tss_fault_handler(regs);
		break;
	case 13:
		gpf_handler(regs);
		break;
	case 14:
		page_fault_handler(regs);
		break;
	default:
		break;
	}
}
