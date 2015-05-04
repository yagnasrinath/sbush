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
#include <sys/fs/fs.h>
#include <sys/sbunix.h>
#include <sys/ProcessManagement/process_helper.h>
#include <sys/ProcessManagement/process_scheduler.h>
#include <sys/ProcessManagement/process.h>
#include <sys/MemoryManagement/virtual_page_allocator.h>
#include <sys/MemoryManagement/kmalloc.h>
#include <sys/MemoryManagement/MemoryManagementUtil.h>
#define ELFMAG0         0x7f
#define ELFMAG1         'E'
#define ELFMAG2         'L'
#define ELFMAG3         'F'
#define LOADABLE 		 1
extern void print_present_pages();
extern void _set_cr3(uint64_t cr3);
static char args[MAX_ARGS][MAX_USR_ARG_LEN];
static char envs[MAX_ENVP][MAX_USR_ENVP_LEN];

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


static void copy_arg_to_stack(task_struct *task, int argc, int envc, BOOL isInitProc)
{
	uint64_t *user_stack, *argv[10], *env[10];
	int len, i;
	uint64_t present_pml4 = read_cr3();
	_set_cr3(task->virtual_addr_space->pml4_t);
	user_stack = (uint64_t*) task->virtual_addr_space->stack_start;
	//kprintf("user stack start is %p \n",user_stack );
	for (i = envc-1; i >= 0; i--) {
		len = kstrlen(envs[i]) + 1;
		user_stack = (uint64_t*)((uint64_t)user_stack - len);
		kmemcpy((char*)user_stack, envs[i], len);
		env[i] = user_stack;
	}
	//kprintf("env[i] end is %p \n",user_stack );
	if(isInitProc) {
		kprintf("process is init process \n");
		len = kstrlen("PATH=bin") + 1;
		user_stack = (uint64_t*)((uint64_t)user_stack - len);
		kmemcpy((char*)user_stack, "PATH=bin", len);
		env[0] = user_stack;
		envc = 1;
		len = kstrlen("HOME=/") + 1;
		user_stack = (uint64_t*)((uint64_t)user_stack - len);
		kmemcpy((char*)user_stack, "HOME=/", len);
		env[1] = user_stack;
		envc = 2;

	}
	//kprintf("env[i] end in case of init is %p \n",user_stack );

	for (i = argc-1; i >= 0; i--) {
		len = kstrlen(args[i]) + 1;
		user_stack = (uint64_t*)((void*)user_stack - len);
		kmemcpy((char*)user_stack, args[i], len);
		argv[i] = user_stack;
	}
	//kprintf("argv[i] end in is %p \n",user_stack );
	// Store the argument pointers
	user_stack--;
	*user_stack  = 0;

	for (i = envc-1; i >= 0; i--) {
		user_stack--;
		*user_stack = (uint64_t)env[i];
	}
	//kprintf("beggining of env is %s \n",user_stack);
	user_stack--;
	*user_stack  = 0;

	//kprintf("argv[i] is %p \n", argv[0]);
	for (i = argc-1; i >= 0; i--) {
		user_stack--;
		*user_stack = (uint64_t)argv[i];
	}
	//kprintf("beggining of argv is %p  %p\n",user_stack, *user_stack);
	user_stack--;

	*user_stack = (uint64_t)argc;
	//kprintf("beggining of argc is %p \n",user_stack);
	//user_stack--;
	//while(1);
	task->virtual_addr_space->stack_start = (uint64_t)user_stack;

	_set_cr3(present_pml4);
}

void load_elf(task_struct* new_task, char* filename,Elf64_Ehdr* elf_header, int argc, int envc, BOOL isInitProc) {

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
		if(programHeader->p_flags == 6) {
			vma_type = DATA;
			vma_perm = READ_WRITE;
		}
		else if (programHeader->p_flags == 5) {
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
	uint64_t user_stack_start = USR_STK_TOP - USR_STK_SIZE;
	vma_struct* new_usr_stack_vma = create_new_vma(0,user_stack_start,USR_STK_TOP,STACK,READ_WRITE);
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
	if(max_addr%PAGE_SIZE) {
		max_addr = (max_addr/PAGE_SIZE)*PAGE_SIZE + PAGE_SIZE;
	}
	heap_start_vaddr= max_addr;
	vma_struct* new_usr_heap_vma =  create_new_vma(0,heap_start_vaddr,heap_start_vaddr+0X8,HEAP,READ_WRITE);
	curr_vma->next = new_usr_heap_vma ;


	kstrncpy(new_task->task_name,filename,TASK_NAME_LENGTH-1 );
	new_task->task_name[TASK_NAME_LENGTH-1] = '\0'; // boundary check
	//new_task->virtual_addr_space->stack_start = user_stack_top-PAGE_SIZE -0x8;
	new_task->virtual_addr_space->stack_start = user_stack_top -0x8;
	new_task->virtual_addr_space->brk_start = heap_start_vaddr;
	new_task->virtual_addr_space->brk_end = heap_start_vaddr;


	copy_arg_to_stack(new_task, argc, envc,isInitProc);
	new_task->state = READY;
	file_des_t * file_d      = (file_des_t * )kmalloc(sizeof(file_des_t));
	file_d->file_type = STDIN_TYPE;
	new_task->fd[0] = file_d;
	file_d      = (file_des_t * )kmalloc(sizeof(file_des_t));
	file_d->file_type = STDOUT_TYPE;
	new_task->fd[1] = file_d;
	file_d      = (file_des_t * )kmalloc(sizeof(file_des_t));
	file_d->file_type = STDERR_TYPE;
	new_task->fd[2] = file_d;
	schedule_process(new_task,new_task->virtual_addr_space->stack_start, elf_header->e_entry );
	//print_present_pages();
}



task_struct * get_elf_task(char *filename, char *argv[], char* env[], BOOL isInitProc) {

	char* data = get_file_data(filename);
	if(data  == NULL) {
		kprintf("data is null \n");
		return NULL;
	}
	Elf64_Ehdr* elf_header = (Elf64_Ehdr*)data;
	if(!is_file_elf(elf_header)) {

		return NULL;
	}
	//uint64_t present_pml4 = read_cr3();

	/*_set_cr3(new_task->virtual_addr_space->pml4_t);
	free_pagetables();
	_set_cr3(present_pml4);*/

	int argc = 0;
	kstrcpy(args[argc++], filename);
	if (argv) {
		while (argv[argc]) {
			kstrcpy(args[argc], argv[argc]);
			argc++;
		}
	}

	int envc =0;
	if (env) {
		while (env[envc]) {
			kstrcpy(envs[envc], env[envc]);
			envc++;
		}
	}
	task_struct* new_task = create_new_task(TRUE);
	if(new_task  == NULL) {
		panic("elfoader.c : get_elf_task : create_new_task returned NULL. Probably out of resources");
	}
	load_elf(new_task, filename,elf_header, argc,envc, isInitProc);
	return new_task;
}
