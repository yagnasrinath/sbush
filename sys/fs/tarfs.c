/*
 * tarfs.c
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/fs/tarfs.h>
#include<sys/sys_generic_util.h>
#include <sys/utils/kstring.h>


char* get_file_data(char* filename) {
	char *p = NULL;
	if(filename == NULL){
		kprintf("Invalid filename");
		return NULL;
	}
	for(HEADER* start= (HEADER*) &_binary_tarfs_start; start < (HEADER*) &_binary_tarfs_end;  ) {
		//kprintf("%s\n", start->name);
		uint64_t file_size = oct_to_dec(atoi(start->size));
		//kprintf("%s\n",start->name);
		if(kstrcmp(filename,start->name) == 0) {
			p = (char*)(start+1);
			//kprintf("File found\n");
			break;
		}
		//kprintf("size of header %d\n",sizeof(HEADER));

		file_size = file_size + sizeof(HEADER);
		if(file_size%512) {
			file_size = ((file_size/512)*512)+512;
		}
		//kprintf("file size is %d",file_size);
		start = (HEADER* )((char *)start + file_size);
	}
	return p;
}

