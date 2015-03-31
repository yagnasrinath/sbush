#include<sys/MemoryManagement/page_tables.h>
#include<sys/MemoryManagement/phy_alloc.h>
#include<sys/utils/string.h>
#include<sys/sbunix.h>
#include<sys/scrn.h>


extern void _set_k_ptable_crm3(uint64_t pml4);

void init_page_tables(void* _physbase,void* _physfree,void*_kernmem)
{
    uint64_t physbase = (uint64_t)_physbase;
    uint64_t physfree = (uint64_t)_physfree;
    uint64_t kernmem = (uint64_t)_kernmem;
    uint64_t pml4 =  (uint64_t)allocate_phy_page();
    uint64_t pdp  =  (uint64_t)allocate_phy_page();
    uint64_t pd   =  (uint64_t)allocate_phy_page();
    uint64_t pt   =  (uint64_t)allocate_phy_page();

    uint64_t pml4_offset = (kernmem << (16            ))>>(9 + 9 + 9 + 12 + 16);
    uint64_t pdp_offset  = (kernmem << (16 + 9        ))>>(9 + 9 + 9 + 12 + 16);
    uint64_t pd_offset  =  (kernmem << (16 + 9 + 9    ))>>(9 + 9 + 9 + 12 + 16);
    uint64_t pt_offset  =  (kernmem << (16 + 9 + 9 + 9))>>(9 + 9 + 9 + 12 + 16);
    // self reference pml4e - recursive mapping
    // equivalent to pml4e[pml4_offset]
    
    *(((uint64_t*)pml4)+pml4_offset-1) = pml4|7;
    *(((uint64_t*)pml4)+pml4_offset) = pdp|7;
    *(((uint64_t*)pdp)+pdp_offset) = pd|7;
    *(((uint64_t*)pd)+pd_offset) = pt|7;
    uint64_t kern_page = physbase&ALIGN_4K;
    uint64_t index=0;
    for(;kern_page<=physfree; kern_page+=PAGE_SIZE,index++)
    {
        *(((uint64_t*)pt)+pt_offset+index) = (kern_page)|3;
    }
    
    printf("index:%d\n",index);
    *(((uint64_t*)pt)+pt_offset+(index)) = 0xb8000|3;
    uint64_t video = 0xffff000000000000|(pml4_offset<<39)|(pdp_offset<<30)|(pd_offset<<21)|((pt_offset+index)<<12);
    update_video(video);
    _set_k_ptable_crm3(pml4);
    printf("video:%p\n",video);
    printf("size%d\n",physfree-physbase);
    printf("physbase:%p\n",physbase);
    printf("physfree:%p\n",physfree);
    printf("kernmem:%p\n",kernmem);
    printf("pml4:%d\n",pml4_offset);
    printf("pdp:%d\n",pdp_offset);
    printf("pd:%d\n",pd_offset);
    printf("pt:%d\n",pt_offset);
    printf("pml4:%p\n",pml4);
    printf("pdp:%p\n",pdp);
    printf("pd:%p\n",pd);
    printf("pt:%p\n",pt);

    //cls();
    printf("worked - physical pages done!!\n");
}
void handle_page_fault(void* fault_addr);
