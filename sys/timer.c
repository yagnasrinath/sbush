/*
 * timer.c
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#include<sys/idt.h>
#include<sys/sbunix.h>
#include<sys/scrn.h>
#include<sys/system.h>
int timer_ticks =0;
int numOfsecs = 0;

void printtimeatrightconer(int value) {

	int sec_length =0;
	char output[33];
	char *ptr;
	ptr = &output[32];
	*ptr = '\0';
	char numberbase[] = "0123456789abcdef";
	ptr--;
	do {
		int currnum = value%10;
		*ptr =numberbase[currnum];
		ptr --;
		sec_length++;
		value = value/10;
	}while(value!=0);
	ptr++;
	putsat(24,  78-sec_length, ptr);
	return ;

}
void timer_handler(struct isr_nrm_regs r)
{
	timer_ticks++;
	if (timer_ticks % 18 == 0)
	{
		numOfsecs++;
		printtimeatrightconer(numOfsecs);
	}
	outportb(0x20, 0x20);
}

