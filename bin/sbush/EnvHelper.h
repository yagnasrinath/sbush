/*
 * PathHelper.h
 *
 *  Created on: Jan 31, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_BIN_SBUSH_ENVHELPER_H_
#define SBUSH_BIN_SBUSH_ENVHELPER_H_
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
typedef struct env {
	char* key;
	char* value;
	struct env* next;
}envList;
envList* initializeEnv(char **inputEnv);
void getValue(char* key, char *val);
void setValue(char* key, char *val);
char * buildEnv(envList *head);
char** getEnv();
void freeEnv(char **envlist);
char* appendPATH(char* PATH,char* appendPATH);
char* prependPATH(char PATH[],char prependPATH[]);

#endif /* SBUSH_BIN_SBUSH_ENVHELPER_H_ */
