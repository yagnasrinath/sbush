/*
 * bushUtils.c
 *
 *  Created on: Feb 1, 2015
 *      Author: ravichandrasadineni
 */

#include "sbushutils.h"

static int isspace(int c) {
    switch(c) {
        case ' ' : return 1;
        case '\t' : return 1;
        case '\v' : return 1;
        case '\f' : return 1;
        case '\r' : return 1;
        case '\n' : return 1;
        default   : return 0;
    }
}
void ltrim(char *s) {
    if(s == 0) {
        return;
    }
    int i;
    for(i=0;isspace(s[i]); i++ ) {
    }
    int k=0;
    while(*(s+i)) {
        *(s+k) = *(s+i);
        i++;
        k++;
    }
    s[k] = '\0';
}

void rtrim(char *s){
    if(s == 0)
        return;
    int i=strlen(s)-1;
    while(isspace(s[i]))
        i--;
    s[i+1] = '\0';
}    

void trim(char *s){
    ltrim(s);
    rtrim(s);
}    


void memset(void* input,int c, int size)
{
    int i=0;
    unsigned char* p = input;
    for(i=0;i<size;i++)
    {
        *p = (unsigned char)c;
        p++;
    }
}

int getcountofchar(char* string, char character) {
    if(string ==0 )
        return 0;
    int len = strlen(string);
    int count =0;
    for(int i=0;i <len; i++) {
        count++;
    }
    return count;
}
int getfirstindex(char* string,char character) {
    if(string ==0) {
        return 0;
    }
    int i=0;
    while(string[i]) {
        if(string[i]==character) {
            return i;
        }
        i++;
    }
    return -1;
}

char** strtokenize(char* string, char character) {
    int numofstrings = getcountofchar(string,character) +1;
    char **output = (char**)malloc(sizeof(char*)*numofstrings);
    int index=0;
    int j=0;
    while(string) {
        j = getfirstindex(string,character);
        if(j==-1){
            char* currstring = (char*)(malloc(strlen(string)+1));
            strcpy(currstring,string);
            output[index] = currstring;
            return output;
        }
        char* currstring = (char*)(malloc(j)+1);
        strncpy(currstring,string,j);
        currstring[j] ='\0';
        string+=j;
        output[index] =currstring;
        index++;
    }
    return 0;
}
