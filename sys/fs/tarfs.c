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
	char *p = NULL;
	if(filename == NULL){
		kprintf("Invalid filename");
		return NULL;
	}
	for(HEADER* start= (HEADER*) &_binary_tarfs_start; start < (HEADER*) &_binary_tarfs_end;  ) {
		uint64_t file_size = convert_oct_dec(start->size);
		if(strcmp(filename,start->name) == 0) {
			p = (char*)(start+1);
			//kprintf("data : %s \n", p);
			break;
		}
		start += 1;

		if(file_size%sizeof(HEADER)) {
			file_size = (file_size/sizeof(HEADER))*sizeof(HEADER)+sizeof(HEADER);
		}
		start = (HEADER* )((char *)start + file_size);
	}
	return p;
}

