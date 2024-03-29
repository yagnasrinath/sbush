#include<sys/sbunix.h>
#include<sys/gdt.h>
#include<sys/fs/tarfs.h>
#include<sys/system.h>
#include<sys/idt.h>
#include<sys/scrn.h>
#include<sys/kb_intrpt_handler.h>
#include<sys/MemoryManagement/phy_alloc.h>
#include<sys/MemoryManagement/page_tables.h>
#include<sys/ProcessManagement/process_scheduler.h>
#include<sys/ProcessManagement/process_helper.h>
int mymain(uint32_t* modulep, void* physbase, void* physfree);
extern task_struct * get_elf_task(char *filename, char *argv[], char *envp[], BOOL isinitproc);
void init_keyboard();
extern void* init_tarfs();
extern void init_pipe();
extern task_struct *init_task_struct;
void start(uint32_t* modulep, void* physbase, void* physfree) {

	while (modulep[0] != 0x9001)
		modulep += modulep[1] + 2;
	/*for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
	 if (smap->type == 1  memory  && smap->length != 0) {
	 printf("Available Physical Memory [%d-%d]\n", smap->base, smap->base + smap->length);
	 }
	 }*/
	mymain(modulep, physbase, physfree);
	// kernel starts here
}

#define INITIAL_STACK_SIZE 8192
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
struct tss_t tss;
//extern BOOL INITSCHEDULING;

int mymain(uint32_t* modulep, void* physbase, void* physfree) {

	init_video(); // video buffer

	idt_install(); // interrupt descriptor table
	timer_install(); // timer
	// activating interrupts
	while (modulep[0] != 0x9001)
		modulep += modulep[1] + 2;
	int num_smaps = 0;

	struct smap_t* smapbase = (struct smap_t*) (modulep + 2);
	for (smap = (struct smap_t*) (modulep + 2);
			smap < (struct smap_t*) ((char*) modulep + modulep[1] + 2 * 4);
			++smap) {
		num_smaps++;
	}
	init_phy_memory(smapbase, num_smaps, physbase, physfree);
	// for mapping free pages on stack
	//physfree += (1024 * 1024); //incrementing by 1MB

	init_page_tables(physbase, physfree, (void*) &kernmem);

	__asm__ __volatile__("movq %0, %%rbp" : :"a"(&stack[0]));
	__asm__ __volatile__("movq %0, %%rsp" : :"a"(&stack[INITIAL_STACK_SIZE]));

	initialize_proc_scheduler();
	initialize_free_list();

	init_keyboard();
	init_tarfs();
	init_pipe();

	//volatile int a=1;
	//while(a==1);

	//INITSCHEDULING = FALSE;
	//create_idle_proc() ;
	init_task_struct = create_init_proc();
	//task_struct* t = create_idle_proc2() ;

	//t->parent = init_task_struct;
	//t->ppid = init_task_struct->pid;


	//create_idle_proc3();
	task_struct* empty_task = get_elf_task("bin/sbush", NULL, NULL,TRUE);
	kstrcpy(empty_task->CWD,"/");
	 //get_elf_task("bin/empty", NULL);
	empty_task->parent = init_task_struct;
	empty_task->ppid = init_task_struct->pid;
	init_task_struct->children_head = empty_task;
	//init_task_struct->children_head->next_sibling = t;
	// empty_task = get_elf_task("bin/tempty", NULL);
	 //empty_task->parent = init_task_struct;
	 	//empty_task->ppid = init_task_struct->pid;
	 	//init_task_struct->children_head = empty_task;
	//printf("Available Physical Memory [%d-%d]\n", smap->base, smap->length);
	__asm__ __volatile__ ("sti");
	//INITSCHEDULING = TRUE;
	while (1)
		;

}


void set_longmode_extension()
{
	uint32_t lo,hi;
	get_MSR(IA32_MSR_EFER,&lo,&hi);
	lo |=(0x1<<8);
	set_MSR(IA32_MSR_EFER,lo,hi);
}



void boot(void) {
	// note: function changes rsp, local stack variables can't be practically used
	register char *s;
	__asm__(
			"movq %%rsp, %0;"
			"movq %1, %%rsp;"
			:"=g"(loader_stack)
			 :"r"(&stack[INITIAL_STACK_SIZE])
	);

	reload_gdt();
	setup_tss();
	start(
			(uint32_t*) ((char*) (uint64_t) loader_stack[3]
														 + (uint64_t) &kernmem - (uint64_t) &physbase), &physbase,
														 (void*) (uint64_t) loader_stack[4]);

	s = "!!!!! start() returned !!!!!";
	kprintf("%s\n", s);
	while (1)
		;
}
