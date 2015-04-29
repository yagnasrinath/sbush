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
#include<sys/utils/kstring.h>
uint16_t special_keys =0;
static uint16_t LEFT_SHIFT_PRESS = 1;
static uint16_t LEFT_SHIFT_RELEASE = ~(-1);
static uint16_t RIGHT_SHIFT_PRESS = 2;
static uint16_t RIGHT_SHIFT_RELEASE = ~(-2);
static volatile uint16_t flag =0;
static volatile uint64_t last_addr;
static volatile uint64_t counter;
static volatile uint64_t num_char_to_read;
static volatile char buf[1024];



void init_keyboard()
{
	special_keys = 0;
    last_addr = 0;
    flag = counter = 0;
}

int gets(uint64_t addr, uint64_t length)
{
	char *user_buf=(char*)addr;
	//int numCharRead = 0;
	num_char_to_read = length;
	flag=1;
	__asm__ __volatile__ ("sti");
	last_addr=get_current_addr();
	while (flag == 1);
	__asm__ __volatile__ ("cli");
	kprintf("buf is %s\n", buf);
	kmemcpy((void *)user_buf, (void *)buf, counter);
	kprintf("user_buf is %s\n", user_buf);
	int count = counter;
	counter = 0;
	num_char_to_read = 0;
	return count;
}

void print_char_pressed(char c) {
	/*char output[3];
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
	 */
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
		char val = get_asci_for_sc(b,is_shift_pressed);
		if (flag == 1) {
			if (val == '\n') {
				buf[counter++] = '\0';
				putch(val);
				flag = 0;
			}
			else if (val == '\b') {
				if (get_current_addr() > last_addr) {
					putch(val);
					counter--;
				}
			} else {
				buf[counter++] = val;
				kprintf("buf is, counter is  %s, %d\n", buf,counter);

				putch(val);
				if(counter == num_char_to_read) {
					flag = 0;
				}
			}
		}
	}
}


