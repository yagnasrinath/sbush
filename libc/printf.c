#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include<string.h>


int isunsignedint = 0;

int putchar(int i) {
	char output[2];
	output[0] = i;
	output[1] = '\0';
	return write(1, output, 1);
}

int puts(char *i) {
	return write(1, i, strlen(i));
}

void printAddress(uint64_t value) {
	int base = 16;
	int isnegative =0;
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
	if(base == 10) {
		if(isnegative == 1 && isunsignedint == 0) {
			*ptr = '-';
			ptr--;
		}
	}
	puts(++ptr);

}


int printIntinStringFormat(int value, int base) {
	int isnegative =0;
	if(value < 0) {
		isnegative =1;
		value *=-1;
	}

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
	if(base == 10) {
		if(isnegative == 1) {
			*ptr = '-';
			ptr--;
		}
	}
	return puts(++ptr);
}

int printf(const char *format, ...) {
	va_list val;
	int numOut;
	int numOfcharPrinted =0;
	char *strOut;
	uint64_t voidprint;
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
		case 'p': voidprint = va_arg(val, uint64_t);
		printAddress(voidprint);
		break;
		case '%': putchar('%');
		break;
		}
		firstargument++;
	}
	va_end( val );
	return printed;
}

