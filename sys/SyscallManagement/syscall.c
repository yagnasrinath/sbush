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
#include<sys/ProcessManagement/process_helper.h>
#include<sys/MemoryManagement/MemoryManagementUtil.h>

void print_present_pages();
uint64_t get_pt_vir_addr(uint64_t);
struct timespec{
	signed int tv_sec;
	long tv_nsec;
};
typedef struct timespec timespec;

enum registers {
	ZERO,
	SS1,
	SS2,
	STK_TOP,
	FLAGS_PROC,
	CS,
	ENTRY,
	LOCAL_VAR,
	RDI,
	RAX,
	RBX,
	RCX,
	RDX,
	RBP,
	RSI,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15


};



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
	//kprintf("pml4t of child is %p \n",child_task->virtual_addr_space->pml4_t );
	//kprintf("pml4t of parent is %p \n",curr_task->virtual_addr_space->pml4_t );
	schedule_process(child_task, curr_task->kstack[KSTACK_SIZE-STK_TOP], curr_task->kstack[KSTACK_SIZE-ENTRY]);
	curr_task->kstack[KSTACK_SIZE-RAX] = child_task->pid;
}


void sys_write(){
	task_struct * curr_task = get_curr_task();
	uint64_t fd = curr_task->kstack[KSTACK_SIZE-RDI];
	uint64_t addr = curr_task->kstack[KSTACK_SIZE-RSI];
	//uint64_t len = curr_task->kstack[KSTACK_SIZE-13];
	//kprintf("File Descriptor is %d\n",fd);
	if(fd == 1 || fd == 2){
		kprintf("%s",addr);
	}
	//kprintf("Printing is done\n");
}



void sys_brk() {
	task_struct * curr_task = get_curr_task();
	uint64_t addr = curr_task->kstack[KSTACK_SIZE-RDI];
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
		curr_task->kstack[KSTACK_SIZE-RAX] = curr_vma->vm_area_end-1;
	}
	else if ( addr >= max_possile_addr) {
		panic("addr  is greater that max possible addr");
		curr_task->kstack[KSTACK_SIZE-RAX]  = -1;
	}
	else {
		curr_task->kstack[KSTACK_SIZE-RAX] = addr;
		if(addr%PAGE_SIZE != 0) {
			addr = PAGE_ALIGN(addr) + PAGE_SIZE;
		}
		curr_vma ->vm_area_end = addr+1;
		//curr_task->kstack[KSTACK_SIZE-10] = curr_vma ->vm_area_end;
	}
	//kprintf("return addr is %p \n", curr_task->kstack[KSTACK_SIZE-RAX]);
}


void sleep() {
	task_struct * curr_task = get_curr_task();
	uint64_t* sleep_for = (uint64_t*)curr_task->kstack[KSTACK_SIZE-RDI];
	uint64_t* rem_time = (uint64_t*)curr_task->kstack[KSTACK_SIZE-RSI];
	timespec* timeSpec = (timespec*)sleep_for ;
	curr_task->state = SLEEP;
	curr_task->sleep_for = timeSpec->tv_sec * 100;
	timeSpec = (timespec*)rem_time ;
	timeSpec->tv_sec = 0;
	timeSpec->tv_nsec = 0;
	curr_task->kstack[KSTACK_SIZE-RAX] = 0;
	__asm__ __volatile__("int $32;");
}



void alarm() {
	task_struct * curr_task = get_curr_task();
	curr_task->state = SLEEP;
	curr_task->sleep_for = curr_task->kstack[KSTACK_SIZE-RDI] * 100;
	curr_task->kstack[KSTACK_SIZE-RAX] = 0;
	__asm__ __volatile__("int $32;");
}


void getpid() {
	task_struct * curr_task = get_curr_task();
	curr_task->kstack[KSTACK_SIZE-RAX] = curr_task->pid;
}


void getppid() {
	task_struct * curr_task = get_curr_task();
	curr_task->kstack[KSTACK_SIZE-RAX] = curr_task->ppid;
}

void waitpid(){
	task_struct * curr_task = get_curr_task();
	task_struct *child_head = curr_task->children_head;
	int wait_for = curr_task->kstack[KSTACK_SIZE-RDI];
	if(curr_task->children_head == NULL){
		curr_task->kstack[KSTACK_SIZE-RAX] = -1;
		return;
	}
	if(wait_for == -1){
		while(child_head != NULL){
			if(child_head->state == ZOMBIE){
				child_head->state  = EXIT;
				curr_task->kstack[KSTACK_SIZE-RAX] = child_head->pid;
				return;
			}
			child_head = child_head->next_sibling;
		}
		curr_task->wait_pid = -1;
		curr_task->state = WAIT;
		__asm__ __volatile__("int $32;");
	}
	else{

		while(child_head != NULL){
			if(child_head->pid == wait_for){
				break;
			}
			child_head = child_head->next_sibling;
		}
		if(child_head != NULL){
			if(child_head ->state == ZOMBIE) {
				child_head ->state  = EXIT;
				curr_task->kstack[KSTACK_SIZE-RAX] = child_head->pid;
				return;
			}
			curr_task->kstack[KSTACK_SIZE-RAX] = child_head->pid;
			curr_task->wait_pid = wait_for;
			curr_task->state = WAIT;
			__asm__ __volatile__("int $32;");
		}
		else{
			curr_task->kstack[KSTACK_SIZE-RAX] = -1;
			return;
		}
	}

}


void print_mem_map (task_struct* curr_task) {
	kprintf("\n##########\n");

	vma_struct * curr_vma = curr_task->virtual_addr_space->vmaList;
	while(curr_vma != NULL) {
		uint64_t vma_start = PAGE_ALIGN(curr_vma->vm_area_start);
		uint64_t vma_end = PAGE_ALIGN(curr_vma->vm_area_end);
		while(vma_start <= vma_end){
			uint64_t* parent_pte_entry = (uint64_t*)get_pt_vir_addr(vma_start);
			if(is_page_present_for_virtual_addr(vma_start)) {
				kprintf("(%p ",vma_start );
				kprintf("%d) ",(*parent_pte_entry)/PAGE_SIZE );
			}
			vma_start += PAGE_SIZE;
		}
		curr_vma  = curr_vma->next;
	}
	kprintf("\n##########\n");
}

void exit(){


	task_struct* curr_task = get_curr_task();
	kprintf("in the sys exit handler %d\n",curr_task->pid);
	/*if(curr_task->pid == 2) {
		//panic("");
	}*/
	//kprintf("process %d cleared \n",curr_task->pid);
	detach_children(curr_task);
	detach_from_parent(curr_task);
	print_mem_map(curr_task);
	free_process_vma_list(curr_task->virtual_addr_space->vmaList);
	kprintf("vma list freed\n");
	//free_pagetables();
	//print_present_pages();

	__asm__ __volatile__("int $32;");
}

void handle_syscall() {
	PUSHA;
	task_struct* curr_task = get_curr_task();
	//kprintf("syscall number %p\n",curr_task->kstack[KSTACK_SIZE-RAX]);
	if(curr_task->kstack[KSTACK_SIZE-RAX] <0 || curr_task->kstack[KSTACK_SIZE-RAX] >19) {
		kprintf("Invalid syscall number %p\n",curr_task->kstack[KSTACK_SIZE-RAX]);
	}
	else {
		switch(curr_task->kstack[KSTACK_SIZE-RAX]) {
		case 12 : alarm();break;
		case 11 : sleep(); break;
		case 10 : waitpid();break;
		case 8  : getppid(); break;
		case 7  : getpid(); break;
		case 6 : fork(); break;
		case 5 : sys_brk(); break;
		case 4 : exit();break;
		case 1 : sys_write();break;
		default : break;
		}
	}
	POPA;
	__asm__ __volatile__("addq $0x08, %rsp;");
	__asm__ __volatile__("iretq");

}

