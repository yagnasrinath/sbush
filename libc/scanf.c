/*
 * scanf.c
 *
 *  Created on: Feb 11, 2015
 *      Author: ravichandrasadineni*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include<string.h>
#define EOF -1
static int numOfCharRead =0;
static char storedChar = -1;


char getCh() {
	char nextChar;
	if(storedChar != -1){
		nextChar = storedChar;
		storedChar = -1;
		return nextChar;
	}
	read(1,&nextChar,1);
	numOfCharRead++;
	return nextChar;

}

void ungetCh(char c) {
	storedChar = c;
}


static int is_space(char ch) {
	if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
		return 1;
	return 0;
}


void readString (char *s) {
	char currChar;
	while(EOF != (currChar =getCh())) {
		if(!is_space(currChar)) {
			break;
		}
	}
	while((EOF != currChar)&&!(is_space(currChar)) ){
		*s = currChar;
		s++;
		currChar = getCh();
	}
	ungetCh(currChar);
	*s = '\0';

}

int convert_into_int(char s,int base) {
	if(s >= '0' && s<='9') return s-'0';
	else if(s == '-') return -1;
	else if (base >10) {
		if(s >= 'a' && s<='f') return s-'a' + 10;
		if(s >= 'A' && s<='F') return s-'A' +10;
		return -2;
	}
	else return -2;
}

int readint(int * output, int base) {
	int num =0;
	int currOut;
	char currChar;
	while(EOF != (currChar =getCh())) {
		if(!is_space(currChar)) {
			break;
		}
	}

	while((EOF != currChar)&&!(is_space(currChar)) ){
		num *= base;

		currOut = convert_into_int(currChar, base);
		if(currOut == -2) {
			ungetCh(currChar);
			return num;
		}
		num +=currOut;

		currChar = getCh();
	}
	*output = num;
	ungetCh(currChar);
	return num;
}


void readchar(char * output, int base) {
	char currChar;
	while(EOF != (currChar =getCh())) {
		if(!is_space(currChar)) {
			break;
		}
	}
	*output= currChar;
}


int scanf(const char *format, ...) {
	numOfCharRead =0;
	va_list val;
	char* charpointer;
	int *numpointer;
	const char *firstargument;
	va_start(val, format);
	firstargument=format;
	while(*firstargument) {
		if(*firstargument != '%') {
			firstargument++;
			continue;
		}
		firstargument++;
		switch(*firstargument) {
		case 'c' :charpointer =va_arg(val,char*);
		readchar(charpointer,10);
		break;


		case 'd' :numpointer=va_arg(val,int*);
		readint(numpointer,10);
		break;


		case 's': charpointer=va_arg(val,char*);
		readString(charpointer);
		break;

		case 'u': numpointer=va_arg(val,int*);
		readint(numpointer,10);
		break;

		case 'x': numpointer=va_arg(val,int*	);
		readint(numpointer,16);
		break;

		}
		firstargument++;
	}
	va_end(val);
	return numOfCharRead;
}


