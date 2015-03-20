#include <sys/sbunix.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>
#include<sys/system.h>
#include<sys/idt.h>
#include<sys/scrn.h>
#include<sys/kb_intrpt_handler.h>
#include<sys/MemoryManagement/phy_alloc.h>
#include<sys/MemoryManagement/page_tables.h>
int mymain(uint32_t* modulep, void* physbase, void* physfree);
void start(uint32_t* modulep, void* physbase, void* physfree)
{

	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	/*for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1  memory  && smap->length != 0) {
			printf("Available Physical Memory [%d-%d]\n", smap->base, smap->base + smap->length);
		}
	}*/
	mymain(  modulep,  physbase,  physfree);
	printf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
	// kernel starts here
}

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
struct tss_t tss;


int mymain(uint32_t* modulep, void* physbase, void* physfree)
{
	init_video();// video buffer
	idt_install(); // interrupt descriptor table
	timer_install(); // timer
	kb_install(); // keyboard
	__asm__ __volatile__ ("sti"); // activating interrupts
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	int num_smaps=0;

	struct smap_t* smapbase = (struct smap_t*)(modulep+2);
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2	*4); ++smap) {
		num_smaps++;
	}
	init_phy_memory( smapbase, num_smaps,  physbase,  physfree );
        // for mapping free pages on stack
        physfree+=(1024*1024); //incrementing by 1MB 

        init_page_tables(physbase,physfree,(void*)&kernmem);
	//printf("Available Physical Memory [%d-%d]\n", smap->base, smap->length);
	while(1);

}


void boot(void)
{
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
			(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
			&physbase,
			(void*)(uint64_t)loader_stack[4]
	);

	s = "!!!!! start() returned !!!!!";
        printf("%s\n",s);
	while(1);
}
