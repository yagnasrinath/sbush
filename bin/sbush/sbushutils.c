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
