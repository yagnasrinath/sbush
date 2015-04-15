/*
 * elfloader.c
 *
 *  Created on: Apr 10, 2015
 *      Author: ravichandrasadineni
 */
#include<sys/defs.h>
#include <sys/elf_loader.h>
#include <sys/system.h>
#include <sys/fs/tarfs.h>
#include <sys/sbunix.h>
#include <sys/ProcessManagement/process_helper.h>
#include <sys/ProcessManagement/process_scheduler.h>
#include <sys/ProcessManagement/process.h>
#include <sys/MemoryManagement/virtual_page_allocator.h>
#define ELFMAG0         0x7f
#define ELFMAG1         'E'
#define ELFMAG2         'L'
#define ELFMAG3         'F'
#define LOADABLE 		 1

extern void _set_cr3(uint64_t cr3);
static char args[MAX_ARGS][MAX_USR_ARG_LEN];
static BOOL is_file_elf(Elf64_Ehdr* elf_loader) {
	if(elf_loader->e_ident[0] !=ELFMAG0 ) {
		return FALSE;
	}
	if(elf_loader->e_ident[1] !=ELFMAG1 ) {
		return FALSE;
	}
	if(elf_loader->e_ident[2] !=ELFMAG2 ) {
		return FALSE;
	}
	if(elf_loader->e_ident[3] !=ELFMAG3 ) {
		return FALSE;
	}
	return TRUE;
}


void copy_arg_to_stack(task_struct *task,char* filename, char* inp_args[]) {
	int argc = 0;
	kstrncpy(args[argc],filename, MAX_USR_ARG_LEN-1);
	args[argc][MAX_USR_ARG_LEN-1]= '\0';
	argc++;
	int curr_arg = 0;
	if(inp_args) {
		while(inp_args[curr_arg]) {
			kstrncpy(args[argc], inp_args[curr_arg], MAX_USR_ARG_LEN-1);
			args[argc][MAX_USR_ARG_LEN-1]= '\0';
			curr_arg++;
			argc++;
		}
	}
	uint64_t present_pml4 = read_cr3();
	_set_cr3(task->virtual_addr_space->pml4_t);
	uint64_t *user_stack = (uint64_t*)task->virtual_addr_space->stack_start;
	uint64_t *argv[10];
	int i = argc-1;
	while(i>=0 ) {
		int len = kstrlen(args[i]) + 1;
		user_stack = (uint64_t*)((void*)user_stack - len);
		kmemcpy(user_stack,args[i], len);
		argv[i] = user_stack;
		i--;
	}
	i = argc-1;
	while(i>=0 ) {
		user_stack--;
		*user_stack = (uint64_t)argv[i] ;
		i--;
	}
	user_stack--;
	*user_stack = (uint64_t)argc;
	task->virtual_addr_space->stack_start= (uint64_t)user_stack;
	_set_cr3(present_pml4);
}

void load_elf(task_struct* new_task, char* filename,Elf64_Ehdr* elf_header, char* argv[]) {

	uint64_t present_pml4 = read_cr3();
	//kprintf("pml4t of new process is %p \n",present_pml4);

	Elf64_Phdr* programHeader = (Elf64_Phdr*)(((char*)elf_header )+ elf_header->e_phoff);
	uint64_t  max_addr =0;
	for(int i=0; i< elf_header->e_phnum; i++) {
		if(programHeader->p_type != LOADABLE) {
			programHeader +=1;
			continue;
		}
		int vma_type;
		int vma_perm ;
		if(programHeader->p_type == 6) {
			vma_type = DATA;
			vma_perm = READ_WRITE;
		}
		else if (programHeader->p_type == 5) {
			vma_type = TXT;
			vma_perm = READ;
		}
		else {
			vma_type = NONE;
			vma_perm = READ_WRITE;
		}
		uint64_t start_addr = programHeader->p_vaddr;
		uint64_t end_vaddr = programHeader->p_vaddr + programHeader->p_memsz;
		if(max_addr < end_vaddr) {
			max_addr  = end_vaddr;
		}
		uint64_t size = programHeader->p_memsz;
		vma_struct* new_vma = create_new_vma(0,start_addr,end_vaddr,vma_type,vma_perm);
		//kprintf("new_vma  succesfully created %p \n", new_vma );
		if(new_task->virtual_addr_space->vmaList == NULL) {
			new_task->virtual_addr_space->vmaList  = new_vma;
		}
		else {
			vma_struct* vma_start = 	new_task->virtual_addr_space->vmaList;
			while(vma_start->next!=NULL) {
				vma_start = vma_start->next;
			}
			vma_start->next = new_vma;
		}
		int page_perm ;
		if(vma_perm == READ_WRITE) {
			page_perm = USER_RW_FLAG| PAGE_PRESENT;
		}
		else {
			page_perm  = USER_RX_FLAG | PAGE_PRESENT;
		}
		// need to change to demand paging
		_set_cr3(new_task->virtual_addr_space->pml4_t);
		ker_mmap(start_addr, size, page_perm|PAGE_PRESENT);
//		kprintf("kern_mmap returned succesfully \n");
		kmemcpy((void*) start_addr, (void*) elf_header + programHeader->p_offset, programHeader->p_filesz);
		// .bss section
		if(programHeader->p_filesz != size) {
			kmemset((void *)start_addr + programHeader->p_filesz, 0, size - programHeader->p_filesz);
		}
		_set_cr3(present_pml4);
		programHeader +=1;
	}
	vma_struct* curr_vma = new_task->virtual_addr_space->vmaList;
	while(curr_vma->next != NULL) {
		curr_vma = curr_vma->next;
	}
	uint64_t user_stack_top = USR_STK_TOP;
	uint64_t user_stack_end = USR_STK_TOP - USR_STK_SIZE;
	vma_struct* new_usr_stack_vma = create_new_vma(0,user_stack_end,USR_STK_TOP,STACK,READ_WRITE);
	//kprintf("stack vma allocated \n");
	curr_vma->next = new_usr_stack_vma;
	// STACK
	// load only one page for the stack. load the rest on request.
	_set_cr3(new_task->virtual_addr_space->pml4_t);
	ker_mmap(user_stack_top - PAGE_SIZE, PAGE_SIZE, USER_RW_FLAG|PAGE_PRESENT);

	//kprintf("stack address allocated \n");
	_set_cr3(present_pml4);
	// HEAP
	curr_vma = curr_vma->next;
	uint64_t heap_start_vaddr ;
	heap_start_vaddr= ((((max_addr - 1) >> 12) + 1) << 12);
	vma_struct* new_usr_heap_vma =  create_new_vma(0,heap_start_vaddr,heap_start_vaddr,HEAP,READ_WRITE);
	curr_vma->next = new_usr_heap_vma ;


	kstrncpy(new_task->task_name,filename,TASK_NAME_LENGTH-1 );
	new_task->task_name[TASK_NAME_LENGTH-1] = '\0'; // boundary check
	new_task->virtual_addr_space->stack_start = user_stack_end - 0x8;
	new_task->virtual_addr_space->brk_start = heap_start_vaddr;
	new_task->virtual_addr_space->brk_end = heap_start_vaddr;
	//copy_arg_to_stack(new_task,filename, argv);
	new_task->state = READY;
	schedule_process(new_task,new_task->virtual_addr_space->stack_start, elf_header->e_entry );
}



task_struct * get_elf_task(char *filename, char *argv[]) {

	char* data = get_file_data(filename);
	if(data  == NULL) {
		kprintf("data is null \n");
		return NULL;
	}
	Elf64_Ehdr* elf_header = (Elf64_Ehdr*)data;
	if(!is_file_elf(elf_header)) {

		return NULL;
	}

	task_struct* new_task = create_new_task(TRUE);

	if(new_task  == NULL) {
		panic("elfoader.c : get_elf_task : create_new_task returned NULL. Probably out of resources");
	}
	load_elf(new_task, filename,elf_header, argv);
	return NULL;
}
