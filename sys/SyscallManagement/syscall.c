/*
 * syscall.
 *
 *  Created on: Apr 19, 2015
 *      Author: harsha
 */
#include<sys/idt.h>
#include<sys/sbunix.h>
#include<sys/utils/kstring.h>
#include<sys/ProcessManagement/process_scheduler.h>
#include<sys/ProcessManagement/process.h>


#define POPA \
		__asm__ __volatile__(\
				"popq %r15;"\
				"popq %r14;"\
				"popq %r13;"\
				"popq %r12;"\
				"popq %r11;"\
				"popq %r10;"\
				"popq %r9;"\
				"popq %r8;"\
				"popq %rsi;"\
				"popq %rbp;"\
				"popq %rdx;"\
				"popq %rcx;"\
				"popq %rbx;"\
				"popq %rax;"\
				"popq %rdi;")

#define PUSHA \
		__asm__ __volatile__(\
				"pushq %rdi;"\
				"pushq %rax;"\
				"pushq %rbx;"\
				"pushq %rcx;"\
				"pushq %rdx;"\
				"pushq %rbp;"\
				"pushq %rsi;"\
				"pushq %r8;"\
				"pushq %r9;"\
				"pushq %r10;"\
				"pushq %r11;"\
				"pushq %r12;"\
				"pushq %r13;"\
				"pushq %r14;"\
				"pushq %r15;")



//9
//10
//11
//12
//13
//14
//15

void fork()  {
	task_struct * curr_task = get_curr_task();
	task_struct * child_task = copy_task_struct(curr_task);
	kmemset(child_task->kstack, 0 , PAGE_SIZE);
	kprintf("pml4t of child is %p \n",child_task->virtual_addr_space->pml4_t );
	kprintf("pml4t of parent is %p \n",curr_task->virtual_addr_space->pml4_t );
	schedule_process(child_task, curr_task->kstack[KSTACK_SIZE-4], curr_task->kstack[KSTACK_SIZE-7]);
	curr_task->kstack[KSTACK_SIZE-10] = child_task->pid;
}


void sys_write(){
	task_struct * curr_task = get_curr_task();
	uint64_t fd = curr_task->kstack[KSTACK_SIZE-9];
	uint64_t addr = curr_task->kstack[KSTACK_SIZE-15];
	//uint64_t len = curr_task->kstack[KSTACK_SIZE-13];
	//kprintf("File Descriptor is %d\n",fd);
	if(fd == 1 || fd == 2){
		kprintf("%s",addr);
	}
	//kprintf("Printing is done\n");
}



void sys_brk() {
	task_struct * curr_task = get_curr_task();
	uint64_t addr = curr_task->kstack[KSTACK_SIZE-9];
	kprintf("passed addr is %p \n", addr);
	uint64_t max_possile_addr = USR_STK_TOP - USR_STK_SIZE;
	vma_struct* curr_vmaList = curr_task->virtual_addr_space->vmaList;
	vma_struct* curr_vma = curr_vmaList;
	while(curr_vma != NULL) {
		if(curr_vma->vma_type == HEAP) {
			break;
		}
		curr_vma = curr_vma->next;
	}
	if(curr_vma == NULL) {
		panic("SYs_break : NO HEAP VMA");
	}
	if(addr ==0) {
		curr_task->kstack[KSTACK_SIZE-10] = curr_vma->vm_area_end;
	}
	else if (addr < curr_vma->vm_area_end || addr >= max_possile_addr) {
		curr_task->kstack[KSTACK_SIZE-10]  = -1;
	}
	else {
		 curr_task->kstack[KSTACK_SIZE-10] = addr;
		if(addr%PAGE_SIZE != 0) {
			addr = PAGE_ALIGN(addr) + PAGE_SIZE;
		}
		curr_vma ->vm_area_end = addr;
		//curr_task->kstack[KSTACK_SIZE-10] = curr_vma ->vm_area_end;
	}
	kprintf("return addr is %p \n", curr_task->kstack[KSTACK_SIZE-10]);
}


void handle_syscall() {
	PUSHA;
	task_struct* curr_task = get_curr_task();
	if(curr_task->kstack[KSTACK_SIZE-10] <0 || curr_task->kstack[KSTACK_SIZE-10] >19) {
		kprintf("Invalid interrupt number \n");
	}
	else {
	 switch(curr_task->kstack[KSTACK_SIZE-10]) {
		 case 6 : fork(); break;
		 case 5 : sys_brk(); break;
		 case 1 : sys_write();break;
	 	 default : break;
		}
	}
	POPA;
	__asm__ __volatile__("addq $0x08, %rsp;");
	__asm__ __volatile__("iretq");

}

