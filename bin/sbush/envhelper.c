/*
 * PathHeper.c
 *
 *  Created on: Jan 31, 2015
 *      Author: ravichandrasadineni
 */

#include "envhelper.h"
static envList * environment;
static int envSize=0;

void parseCurrEnv(envList* currentEnv, char* inputEnv) {
	if(inputEnv ==0) {
		printf("ENV HELPER.c: parseCurrEnv: FATAL : ENVIRONMENT contains null values\n");
		exit(1);
	}
	int i;

	//checking for the = sign.
	for(i=0; inputEnv[i] != 0; i++) {
		if(inputEnv[i] == '=') {
			break;
		}
	}
	int len = strlen(inputEnv);
	char *key = (char*)malloc(i+1);
	strncpy(key,inputEnv,i);
	currentEnv->key = key;
	char *value =(char*)malloc(len-(i));
	strcpy(value,(inputEnv+i+1));
	currentEnv->value = value;
}


void initializeEnv(char **inputEnv) {

	if(inputEnv == 0) {
		return ;
	}
	int i =0;
	envList* head = 0;
	envList* tail = 0;
	while(inputEnv[i]) {
		envList* currentEnv = (envList*)malloc(sizeof(envList));
		if(head == 0) {
			head= currentEnv ;
		}
		else {
			tail->next=currentEnv ;
		}
		tail = currentEnv;
		currentEnv->next = 0;
		parseCurrEnv(currentEnv,inputEnv[i]);
		envSize++;
	}
	environment = head;
}

void getValue(char* key, char *val) {
	if(key == 0) {
		printf("PATH HELPER.c:getValue : FATAL : Key passed is null\n");
	}
	envList *head =  environment;
	while(head != 0) {
		if(strcmp(head->key,key) == 0) {
			strcpy(val,head->value);
		}
		head = head->next;
	}
}

void setValue(char* key, char *val) {
	if(key == 0) {
		printf("ENV HELPER.c: setValue: FATAL : Key passed is null\n");
	}
	if(val == 0) {
		printf("ENV HELPER.c: setValue: FATAL : val passed is null\n");
	}
	envList *head =  environment;
	while(head != 0) {
		if(strcmp(head->key,key) == 0) {
			free(head->value);
			head->value = (char *)malloc(strlen(val)+1);
			strcpy(head->value,val);
		}
		head = head->next;
	}
}

char * buildEnv(envList *head) {
	int len = strlen(head->key) + strlen(head->value) + 2;
	char * envVar = (char *)malloc(sizeof(len));
	strcpy(envVar,head->key);
	strcpy(envVar,"=");
	strcpy(envVar,head->value);
	return envVar;
}
char ** getEnv() {
	envList *head =  environment;
	char ** env = (char **)malloc(sizeof(char *)*(envSize+1));
	int i=0;
	while(head !=0) {
		env [i] = buildEnv(head);
		head = head->next;
		i++;
	}
	env [i]= 0;
	return env;
}

void freeEnv(char **envlist){
	int i = 0;
	while(envlist[i]!= 0){
		free(envlist[i]);
		i++;
	}
	free(envlist);
}

char* appendPATH(char* PATH,char* appendPATH){
	int newLength = strlen(PATH) + strlen(appendPATH) + 1;
	char *newPATH = (char *)malloc(sizeof(newLength)) ;
	strcpy(newPATH, PATH);
	strncpy(newPATH+strlen(PATH), appendPATH, strlen(appendPATH)+1);
	return newPATH;
}

char* prependPATH(char PATH[],char prependPATH[]){
	int newLength = strlen(PATH) + strlen(prependPATH) + 1;
	char *newPATH;
	newPATH = (char *)malloc(newLength);
	strcpy(newPATH, prependPATH);
	strncpy(newPATH+strlen(prependPATH), PATH, strlen(PATH)+1);
	return newPATH;
}
