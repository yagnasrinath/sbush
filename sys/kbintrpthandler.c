/*
 * kbintrpthandler.c
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#include <sys/kbsctochar.h>
#include <sys/scrn.h>
#include <sys/system.h>
#include<sys/sbunix.h>
#include<sys/idt.h>
uint16_t special_keys =0;
static uint16_t LEFT_SHIFT_PRESS = 1;
static uint16_t LEFT_SHIFT_RELEASE = ~(-1);
static uint16_t RIGHT_SHIFT_PRESS = 2;
static uint16_t RIGHT_SHIFT_RELEASE = ~(-2);


void print_char_pressed(char c) {
	char output[3];
	if(c == '\t') {
		output[0] = '\\';
		output[1] ='t';
		output[2] ='\0';
	}
	else if(c == '\n') {
		output[0] = '\\';
		output[1] ='n';
		output[2] ='\0';
	}
	else if(c == '\b') {
		output[0] = '\\';
		output[1] ='b';
		output[2] ='\0';
	}
	else {
		output[0]=c;
		output[1]=' ';
		output[2]='\0';
	}
	putsat(24,79,output);
}


void  kb_intrpt_handler( struct isr_nrm_regs  stack) {
	uint8_t b = inportb(0x60);
	if(b == SC_LEFTSHIFT) {
		special_keys = special_keys | LEFT_SHIFT_PRESS;
	}
	else  if(b == (SC_LEFTSHIFT | 0X80)) {
		special_keys = special_keys & LEFT_SHIFT_RELEASE;
	}
	else if (b == SC_RIGHTSHIFT ) {
		special_keys = special_keys | RIGHT_SHIFT_PRESS;
	}
	else if (b == (SC_RIGHTSHIFT  |0X80)) {
		special_keys = special_keys & RIGHT_SHIFT_RELEASE;
	}
	else {
		BOOL is_shift_pressed = (special_keys && LEFT_SHIFT_PRESS) ||
				(special_keys && RIGHT_SHIFT_PRESS);

		print_char_pressed(get_asci_for_sc(b,is_shift_pressed));
	}
}


