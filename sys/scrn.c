/*
 * scrn.c
 *
 *  Created on: Feb 28, 2015
 *      Author: ravichandrasadineni
 */
#include <sys/system.h>
#include "../include/sys/utils/kstring.h"
#define MAX_ROW 24
#define MAX_COLUMN 80
static unsigned short *basememptr;
static int colorattr = 0X0F;
static uint64_t base_video_addr;

static int csr_column = 0, csr_row = 0;

uint64_t get_current_addr() {
	return base_video_addr + csr_row*MAX_COLUMN + csr_column;

}


static void move_csr() {
	unsigned current_pos  = csr_row*MAX_COLUMN + csr_column;
	outportb(0x3D4, 14);
	outportb(0x3D5, current_pos >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, current_pos);
}

static void scroll() {
	unsigned short empty, copyfrom;

	if(csr_row >=MAX_ROW) {
		copyfrom = csr_row-MAX_ROW+1;
		kmemcpy(basememptr,basememptr+copyfrom*MAX_COLUMN,(csr_row-copyfrom)*MAX_COLUMN*2 );
		empty = 0x20|colorattr<<8;
		kmemsetw(basememptr+(csr_row-copyfrom)*MAX_COLUMN,empty,MAX_COLUMN);
		csr_row=MAX_ROW-1;
		csr_column=0;
	}
}

void putchat(int put_row , int put_column,unsigned char c ) {
	 if(put_row > 25 || put_row < 1 || put_column <1 ||put_column > 80 ) {
		 	 return;
	 }
	 unsigned short * current_pos = basememptr+put_row*80+put_column-1;
	 *current_pos = c|colorattr<<8;
}

void putsat(int put_row , int put_column, char* s ) {
	 if(put_row > 25 || put_row < 1 || put_column <1 ||put_column > 80 ) {
		 	 return;
	 }
	 while(*s) {
		 putchat(put_row,put_column++,*s++);
		 if(put_column > 80) {
			 put_row++;
			 put_column =0;
		 }
	 }
}

void cls() {
	unsigned short blank;
	blank = 0X20 | colorattr << 8;
	for(int i=0;i<MAX_ROW;i++) {
		kmemsetw(basememptr+i*80,blank,80);
	}
	csr_row = 0;
	csr_column =0;
	move_csr();

}
void putch(unsigned char c) {
	// back space
	if(c == 0X08) {
		if(csr_column != 0) {
			unsigned short blank = 0X20 | colorattr<<8;
			unsigned short * current_pos = basememptr+csr_row*80+csr_column-1;
			*current_pos = blank;
			csr_column--;
		}
	}
	// tab space
	else if(c == 0X09) {
		// move the bit position by 8 and remove last 3 bits
		//csr_column = (csr_column + 8) | ~(8-1);
		csr_column = (csr_column + 8);
	}
	// carriage return
	else if (c == 0X0D) {
		csr_column = 0;
	}
	else if (c == '\n') {
		csr_column = 0;
		csr_row ++;
	}
	else if (c >= 0X20) {
		unsigned short* current_pos = (basememptr+ csr_row*80 +csr_column );
		*current_pos = c|colorattr<<8;
		csr_column++;
	}
	if(csr_column >= MAX_COLUMN) {
		csr_column = 0;
		csr_row++;
	}

	scroll();
	move_csr();

}
void puts(char *str) {
	while(*str) {
		putch(*str++);
	}

}
void settextcolor(unsigned char forecolor, unsigned char backcolor) {
	colorattr = (backcolor<<4|(forecolor&0X0F))	;

}
void init_video() {
	basememptr = (unsigned short *)0xb8000;
	cls();
}


void update_video(uint64_t addr) {
	basememptr = (unsigned short *)addr;
	base_video_addr = addr;
	cls();
}


