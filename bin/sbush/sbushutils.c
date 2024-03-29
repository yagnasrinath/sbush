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
    if(s == 0)
        return;
    ltrim(s);
    rtrim(s);
}



int getcountofchar(char* string, char character) {
    if(string ==0 )
        return 0;
    int len = strlen(string);
    int count =0;
    for(int i=0;i <len; i++) {
        if(*string == character)
        {
            count++;
        }
        string++;
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
    char **output = (char**)malloc(sizeof(char*)*numofstrings+1);
    //printf("%d\n",numofstrings);
    output[numofstrings] = 0;
    int index=0;
    int j=0;
    while(string) {
        //printf("%s\n",string);
        j = getfirstindex(string,character);
        if(j==-1){
            char* currstring = (char*)(malloc(strlen(string)+1));
            memset(currstring,'\0',strlen(string)+1);
            strcpy(currstring,string);
            //printf("curr string : %p\n",currstring);
            output[index] = currstring;
            //printf("next string : %p\n",output[0]);
           // printf("next string : %p\n",output[1]);
            return output;
        }
        char* currstring = (char*)(malloc(j+1));
        memset(currstring,'\0',j+1);
        strncpy(currstring,string,j);
        currstring[j] ='\0';
        //printf("curr string : %p\n",currstring);
        string+=(j+1);
        output[index] =currstring;
        index++;
    }
    return 0;
}


void free_str_tokens(char**originallist)
{
    char ** _tmp = originallist;
    if(_tmp==0)
    {
        return;
    }
    while(*_tmp)
    {
        free(*_tmp);
        _tmp++;
    }
    if(originallist)
    {
        free(originallist);
    }
}
