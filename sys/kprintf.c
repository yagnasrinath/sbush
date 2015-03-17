#include <sys/sbunix.h>
#include <stdarg.h>
#include<sys/system.h>
#include<sys/scrn.h>
int lines = 0;
int columns = 0;
int color = 7;
int isunsignedint = 0;


void printIntinStringFormat(int value, int base) {
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
        if(isnegative == 1 && isunsignedint == 0) {
            *ptr = '-';
            ptr--;
        }
    }
    puts(++ptr);
}

void printf(const char *format, ...) {
    va_list val;
    int numOut;
    char *strOut;
    uint64_t voidprint;
    const char *firstargument;
    va_start(val, format);
    firstargument=format;
    while(*firstargument) {
        if(*firstargument != '%') {
            putch(*firstargument);
            firstargument++;
            continue;
        }
        firstargument++;
        switch(*firstargument) {
            case 'c' :numOut =va_arg(val,int);
                      putch(numOut);
                      break;
            case 'd' :numOut=va_arg(val,int);
                      printIntinStringFormat(numOut,10);
                      break;
            case 's': strOut=va_arg(val,char*);
                      puts(strOut);
                      break;
            case 'u': numOut=va_arg(val,int);
                      isunsignedint = 1;
                      printIntinStringFormat(numOut,10);
                      isunsignedint = 0;
                      break;
            case 'x': numOut=va_arg(val,int);
                      printIntinStringFormat(numOut,16);
                      break;
            case 'p': voidprint = va_arg(val, uint64_t);
                      printIntinStringFormat(voidprint,16);
                      break;
            case '%': putch('%');
                      break;
        }
        firstargument++;
    }
    va_end( val );
    return;
}

