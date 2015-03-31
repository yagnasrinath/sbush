/*
 *
 *  Created on: Mar 12, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_SYS_MEMOYMANAGEMENT_PAGE_TABLES_H_
#define SBUSH_SYS_MEMOYMANAGEMENT_PAGE_TABLES_H_

#include<sys/defs.h>
#include<sys/MemoryManagement/MemoryManagementUtil.h>

void init_page_tables(void* physbase,void* physfree,void* kernmem);
void handle_page_fault(void* fault_addr);



#endif /* SBUSH_SYS_MEMOYMANAGEMENT_PAGE_TABLES_ */
