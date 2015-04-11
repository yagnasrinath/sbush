#include<sys/MemoryManagement/page_tables.h>
#include<sys/MemoryManagement/phy_alloc.h>
#include<sys/MemoryManagement/MemoryManagementUtil.h>
#include<sys/sbunix.h>
#include<sys/scrn.h>
#include "../../include/sys/utils/kstring.h"


extern void _set_cr3(uint64_t pml4);


void init_page_tables(void* _physbase,void* _physfree,void*_kernmem)
{
    uint64_t physbase = (uint64_t)_physbase;
    uint64_t physfree = (uint64_t)_physfree;
    uint64_t kernmem = (uint64_t)_kernmem;

    uint64_t pml4 =  (uint64_t)allocate_phy_page();
    uint64_t pdp  =  (uint64_t)allocate_phy_page();
    uint64_t pd   =  (uint64_t)allocate_phy_page();
    uint64_t pt   =  (uint64_t)allocate_phy_page();

    uint64_t pml4_offset = (kernmem << (16            ))>>(9 + 9 + 9 + 12 + 16); //511
    uint64_t pdp_offset  = (kernmem << (16 + 9        ))>>(9 + 9 + 9 + 12 + 16);
    uint64_t pd_offset  =  (kernmem << (16 + 9 + 9    ))>>(9 + 9 + 9 + 12 + 16);
    uint64_t pt_offset  =  (kernmem << (16 + 9 + 9 + 9))>>(9 + 9 + 9 + 12 + 16);
    // self reference pml4e - recursive mapping
    // equivalent to pml4e[pml4_offset]
    
    *(((uint64_t*)pml4)+pml4_offset-1) = pml4 | KERNEL_RW_FLAG |PAGE_PRESENT ;
    *(((uint64_t*)pml4)+pml4_offset) = pdp | KERNEL_RW_FLAG| PAGE_PRESENT;
    *(((uint64_t*)pdp)+pdp_offset) = pd | KERNEL_RW_FLAG| PAGE_PRESENT;
    *(((uint64_t*)pd)+pd_offset) = pt | KERNEL_RW_FLAG| PAGE_PRESENT;

    uint64_t kern_page = physbase&ALIGN_4K;
    uint64_t index=0;
    for(;kern_page<=physfree; kern_page+=PAGE_SIZE,index++)
    {
        *(((uint64_t*)pt)+pt_offset+index) = (kern_page) | KERNEL_RW_FLAG |PAGE_PRESENT;
    }
    
    kprintf("index:%d\n",index);
    *(((uint64_t*)pt)+pt_offset+(index)) = 0xb8000 | USER_RW_FLAG | PAGE_PRESENT;
    uint64_t video = 0xffff000000000000|(pml4_offset<<39)|(pdp_offset<<30)|(pd_offset<<21)|((pt_offset+index)<<12);
    index++;
    update_video(video);
    _set_cr3(pml4);
    kprintf("video:%p\n",video);
    kprintf("size%d\n",physfree-physbase);
    kprintf("physbase:%p\n",physbase);
    kprintf("physfree:%p\n",physfree);
    kprintf("kernmem:%p\n",kernmem);
    kprintf("pml4:%d\n",pml4_offset);
    kprintf("pdp:%d\n",pdp_offset);
    kprintf("pd:%d\n",pd_offset);
    kprintf("pt:%d\n",pt_offset);
    kprintf("pml4:%p\n",pml4);
    kprintf("pdp:%p\n",pdp);
    kprintf("pd:%p\n",pd);
    kprintf("pt:%p\n",pt);
    cls();
    kprintf("worked - physical pages done!!\n");
    set_present_virtual_address(kernmem + index*PAGE_SIZE);
    // saves the kernel cr3
    _set_cr3(pml4);
    //saves the kernel mapping to save it to the process
    set_kernel_pml4_entry(pdp | KERNEL_RW_FLAG|PAGE_PRESENT);
}




