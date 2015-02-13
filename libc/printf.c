#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include<string.h>

int putchar(int i) {
	char output[2];
	output[0] = i;
	output[1] = '\0';
 	return write(1, output, 1);
}

int puts(char *i) {
	return write(1, i, strlen(i));
}

int printIntinStringFormat(int value, int base) {
	char output[33];
	char *ptr;
	ptr = &output[32];
	*ptr = '\0';
	char numberbase[] = "0123456789abcdef";
	ptr--;
	do {
		int currnum = value%base;
		*ptr =numberbase[currnum];
		ptr --;
		value = value/base;
	}while(value!=0);
	if(base == 16) {
		*ptr='X';
		ptr--;
		*ptr='0';
		ptr--;
	}
	if(base == 8) {
			*ptr='0';
			ptr--;
		}
	return puts(++ptr);
}

int printf(const char *format, ...) {
	va_list val;
	int numOut;
	int numOfcharPrinted =0;
	char *strOut;
	int printed = 0;
	const char *firstargument;
	va_start(val, format);
	firstargument=format;
	while(*firstargument) {
		if(*firstargument != '%') {
			putchar(*firstargument);
			firstargument++;
			continue;
		}
		firstargument++;
		switch(*firstargument) {
		case 'c' :numOut =va_arg(val,int);
				  numOfcharPrinted+=putchar(numOut);
				  break;
		case 'd' :numOut=va_arg(val,int);
				  numOfcharPrinted+=printIntinStringFormat(numOut,10);
				  break;
		case 's': strOut=va_arg(val,char*);
				  numOfcharPrinted+=puts(strOut);
				  break;
		case 'u': numOut=va_arg(val,int);
				  numOfcharPrinted+=printIntinStringFormat(numOut,10);
				  break;
		case 'x': numOut=va_arg(val,int);
				  numOfcharPrinted+=printIntinStringFormat(numOut,16);
				  break;
		case '%': putchar('%');
				  break;
		}
		firstargument++;
	}
	va_end( val );
	return printed;
}
