/*
 * PathHelper.h
 *
 *  Created on: Jan 31, 2015
 *      Author: ravichandrasadineni
 */

#ifndef ENVHELPER_H
#define ENVHELPER_H
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"sbushutils.h"
#include"sbconstants.h"
typedef struct env {
	char* key;
	char* value;
	struct env* next;
}envList;
void initializeenv(char **inputEnv);
void getvalue(char* key, char *val);
void setvalue(char* key, char *val);
void buildenv();
char*** getenv();
void freeenv(char **envlist);
char* appendpath(char* PATH,char* appendPATH);
char* prependpath(char PATH[],char prependPATH[]);

#endif /* SBUSH_BIN_SBUSH_ENVHELPER_H_ */
