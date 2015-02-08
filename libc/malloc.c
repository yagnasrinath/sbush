#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#define AL_SIZE(X) ((((X-1)>>3)<<3)+8)
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
    blockptr newblk = (blockptr)sbrk(0);
    if(last!=NULL)
    {
        last->next = newblk;
    }
    if(sbrk(BLOCK_SIZE + size)==(void*)-1)
    {
        return NULL;
    }
    newblk->prev = last;
    newblk->free = 0;
    newblk->next = NULL;
    return newblk;
}

void split_blk(blockptr cur,size_t req_size)
{
    int rem_blk_data_size = cur->size - req_size - BLOCK_SIZE;
    blockptr new =(blockptr) (cur->data + cur->size);
    new->size = rem_blk_data_size;
    new->next = cur->next;
    new->free = 1;
    cur->next = new;
    cur->size = req_size;
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
            }
            mem = new->data;
        }
    }
    else
    {
        base = allocate_new_memory(NULL,size);
        if(base!=NULL)
        {
            mem = base->data;
        }
    }
    return mem;
}
