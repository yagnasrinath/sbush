/*
 * tarfs.c
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/fs/tarfs.h>

static uint64_t get_highest_dec_pos(uint64_t input) {
	int max_dec = 1;
	while(input > 0) {
		input = input/10;
		max_dec= max_dec*10;
	}
	return max_dec/10;
}

static uint64_t convert_to_int(char inputChar[12]) {
	uint64_t output = 0;
	for(int i=0; i <11; i++) {
		output = output*10 + (inputChar[i]- '0');
	}

	return output;
}
static uint64_t convert_oct_dec(char inputChar[12]) {
	int input =convert_to_int(inputChar);
	if(input == 0 ) return 0;
	uint64_t output = 0;
	uint64_t hig_dec_pos = get_highest_dec_pos(input);
	while((input > 0) &&(hig_dec_pos!=0)) {
		int curr_num = input/hig_dec_pos;
		output = output*8 +  curr_num;
		input = input%hig_dec_pos;
		hig_dec_pos = hig_dec_pos/10;

	}
	return output;
}


char* get_file_data(char* filename) {
	HEADER *header = (HEADER*) &_binary_tarfs_start;
	    int size_of_file = 0;
	    char *p;

	    do {
	        // Convert header->size string to octal to decimal
	        size_of_file = convert_oct_dec((header->size));
	        p = (char*)(header + 1);

	        //kprintf("\n...File Name: %s..File Size: %d bytes..Type %s: ", header->name, size_of_file, header->typeflag);
	        if (kstrcmp(filename, header->name) == 0) {
	            return (void*)p;
	        }

	        if (size_of_file > 0) {
	            header = header + size_of_file / (sizeof(HEADER) + 1) + 2;
	        } else {
	            header = header + 1;
	        }
	    } while(header < (HEADER*)&_binary_tarfs_end);

	    // kprintf("\nFile not found");
	    return NULL;
}

