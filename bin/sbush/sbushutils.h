/*
 * bushUtils.h
 *
 *  Created on: Feb 1, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSHUTILS_H
#define SBUSHUTILS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void rtrim(char *s);
void ltrim(char* input);
void trim(char* input);
void memset(void* input,int c,int size);
int getCountOfChar(char* string, char character);
int getfirstindex(char* string,char character);
char** strtokenize(char* string, char character);

#endif /* SBUSH_BIN_SBUSH_LIB_BUSHUTILS_H_ */
