#include<stdio.h>
#include<stdlib.h>
#define AL_SIZE(X) ((((X-1)>>3)<<3)+8)
#define BLOCK_SIZE 20

struct mem_block {
	size_t size;
	struct mem_block  *next;
	struct mem_block  *prev;
	int free;
	void *ptr;
	char data [1];
};

typedef struct mem_block* blockptr;

blockptr last, brk_start;

blockptr find_suitable_block(blockptr *last, size_t size) {
	if(brk_start == 0) {
		return 0;
	}
	blockptr tmp = brk_start;
	while(tmp &&!(tmp->size >= size && tmp->free)){
		*last = tmp;
		tmp  = tmp->next;

	}
	if(tmp == 0){
		return 0;
	}
	return tmp;
}

blockptr allocate_new_mememory(blockptr last, size_t size) {
	blockptr new = sbrk(0);
	last->next = new;
	sbrk(BLOCK_SIZE + size);
	new->prev = last;
	new->free = 0;
	return 0;
}



void *malloc(size_t size)
{
	return 0;
}
