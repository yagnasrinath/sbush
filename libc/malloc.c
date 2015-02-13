#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>
#define ALIGN_SIZE(X) ((((X-1)>>3)<<3)+8)
#define BLOCK_SIZE 40



struct mem_block 
{
	//packing structure to multiple of 8 as the machine is 64 bit
	size_t size; //8bytes
	struct mem_block  *next;//8bytes
	struct mem_block  *prev;//8bytes
	long free;//8bytes 0-False !0-True
	void *ptr;//8bytes
	char data[1];
};

typedef struct mem_block* blockptr;

blockptr base = NULL;

// last is used to extend_heap when the holes cannot be used to
// alloacate requested size and the new last address after sbrk
// can be directly updated using last instead of looping again
// find last

void* sbrk(int size)
{

	void * cur = (void*)syscall_1((uint64_t)SYS_brk,(uint64_t)0);
	if(cur==(void*)-1||size==0)
	{
		return cur;
	}
	else
	{

		return (void*)syscall_1((uint64_t)SYS_brk,(uint64_t)((char*)cur+size));
	}
}

int brk(void * addr) {
	return (int)syscall_1((uint64_t)SYS_brk,(uint64_t)addr);
}

blockptr find_suitable_block(blockptr *last, size_t size) {
	blockptr tmp = base;
	while(tmp &&!(tmp->size >= size && tmp->free)){
		*last = tmp;
		tmp  = tmp->next;
	}
	if(tmp == NULL){
		return NULL;
	}
	return tmp;
}

blockptr allocate_new_memory(blockptr last, size_t size) {
	blockptr newblk = sbrk(0);
	if(last!=NULL)
	{
		last->next = newblk;
	}
	if((void*)sbrk(BLOCK_SIZE + size)==(void*)-1)
	{
		return NULL;
	}
	newblk->prev = last;
	newblk->free = 0;
	newblk->next = NULL;
	newblk->size = size;
	return newblk;
}

void split_blk(blockptr cur,size_t req_size)
{
	int rem_blk_data_size = cur->size - req_size - BLOCK_SIZE;
	blockptr new =(blockptr) (cur->data + req_size);
	new->size = rem_blk_data_size;
	new->next = cur->next;
	new->prev = cur;
	new->free = 1;
	cur->next = new;
	cur->size = req_size;
	if(cur->next != NULL) {
		cur->next->prev= new;
	}
}

void *malloc(size_t size)
{
	void* mem = NULL;
	size = ALIGN_SIZE(size);
	blockptr new,last;
	if(base)
	{
		new = find_suitable_block(&last,size);
		if(new == NULL)
		{
			new = allocate_new_memory(last,size);
			if(new!=NULL)
			{
				mem = new->data;
			}
		}
		else
		{
			if(new->size>size+2*BLOCK_SIZE+8)
			{
				split_blk(new,size);
				new->free =0;
			}
			mem = new->data;
		}
	}
	else
	{
		base = allocate_new_memory(NULL,size);
		new = base;
		if(base!=NULL)
		{
			mem = base->data;
		}
	}
	if(mem!=NULL)
	{
		new->ptr=mem;
	}
	return mem;
}


void merge_blks(blockptr cur)
{
	if(cur->next!=NULL &&cur->next->free==1)
	{
		cur->size=cur->next->size+BLOCK_SIZE+cur->size;
		if(cur->next->next!=NULL)
		{
			cur->next->next->prev=cur;
			cur->next=cur->next->next;
		}
		else {
			cur->next = NULL;
		}
	}
	else if(cur->next == NULL)
	{
		if((void*)sbrk(-1*(BLOCK_SIZE + cur->size))==(void*)-1)
		{
			//errno = -1;
		}
	}
}

blockptr get_blk(void*mem)
{
	char* temp = (char*)mem;
	return (blockptr)(temp-BLOCK_SIZE);
}

blockptr get_valid_blk(void*mem)
{
	blockptr cur = get_blk(mem);
	void* ptr;
	if((ptr = (void*)sbrk(0))!=(void*)-1)
	{
		if(base!=NULL && base<=cur&&(char*)((char*)ptr-BLOCK_SIZE)>=(char*)cur && cur->free==0&&(cur->ptr==mem))
		{
			return cur;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		//ideally should not return -1 for sysbrk(0)
		return NULL;
	}
}

void free(void* mem)
{
	blockptr cur = (blockptr)get_valid_blk(mem);
	if(cur==NULL)
	{
		//set errno
		//return -1;
		return;
	}
	else
	{
		cur->free=1;
		if(cur->prev!=NULL && cur->prev->free==1)
		{
			merge_blks(cur->prev);
			cur = cur->prev;
		}
		if(cur->next != NULL) {
			merge_blks(cur);
		}
		else {
			if(cur->prev != NULL) {
				cur->prev->next = NULL;
			}
			else {
				base = NULL;
			}
			brk(cur);
		}
	}
}
