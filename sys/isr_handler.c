/*
 * isr_handler.c
 *
 *  Created on: Apr 15, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/defs.h>
#include<sys/idt.h>
#include<sys/sbunix.h>
#include<sys/ProcessManagement/process_scheduler.h>
#include<sys/MemoryManagement/virtual_page_allocator.h>

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
	panic("Divide By Zero Error %d");
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
	kprintf("protection_fault_handler rsp %p \n",lrsp);
	print_regiters(regs);
	panic("Protection fault handler");
}

void page_fault_handler(struct isr_nrm_regs regs) {
	uint64_t lcr2 = 3;
	__asm__ __volatile__ ("movq %%cr2, %0;" : "=r"(lcr2));
	/*uint64_t lcr3 =3;
	__asm__ __volatile__ ("movq %%cr3, %0;" : "=r"(lcr3));
	uint64_t lrsp =3;
	__asm__ __volatile__ ("movq %%rsp, %0;" : "=r"(lrsp));*/
	//kprintf("\n page_fault_handler cr3 %p \n",lcr3);
	kprintf("page_fault_handler cr2 %p \n",lcr2);
	//kprintf("page_fault_handler rsp %p \n",lrsp);
	//print_regiters(regs);
	uint64_t fault_addr = lcr2;
	if(lcr2 >= USR_STK_TOP) {
		panic("PAGE FAULT IN KERNEL\n");
	}
	if(regs.error & 0x1) { // PAGE PRESENT

	}
	else {
		task_struct* curr_task = get_curr_task();
		vma_struct* curr_vmaList = curr_task->virtual_addr_space->vmaList;
		if(curr_vmaList  == NULL) {
			panic("No vma list for the process");
		}
		vma_struct* curr_vma = curr_vmaList;
		fault_addr = PAGE_ALIGN(fault_addr);
		kprintf("Fault address is %p\n",fault_addr );
		while(curr_vma != NULL) {
			uint64_t curr_start = curr_vma->vm_area_start;
			uint64_t curr_end = curr_vma->vm_area_end;
			//kprintf("VMA start %p\n",curr_start );
			//kprintf("VMA end %p\n",curr_end );

			if((fault_addr  >= curr_start) && (fault_addr <= curr_end)) {
				ker_mmap(fault_addr, PAGE_SIZE, PAGE_PRESENT | USER_RW_FLAG);
				break;
			}
			curr_vma = curr_vma->next;
		}
		if(curr_vma == NULL) {
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
