/*
 * scrn.h
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_SCRN_H_
#define SBUSH_INCLUDE_SYS_SCRN_H_

void cls();
void putch(unsigned char c);
void puts(char *str);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
void init_video();
void update_video(uint64_t addr);
void putchat(int put_row , int put_column,unsigned char c );
void putsat(int put_row , int put_column, char* s );
uint64_t get_current_addr();

#endif /* SBUSH_INCLUDE_SYS_SCRN_H_ */
