/*
 * PathHeper.c
 *
 *  Created on: Jan 31, 2015
 *      Author: ravichandrasadineni
 */

#include "envhelper.h"
extern envList * environment;
extern int envSize;
extern char** env;
void parsecurrenv(envList* currentEnv, char* inputEnv) {
	if(inputEnv ==0) {
		printf("ENV HELPER.c: parsecurrenv: FATAL : ENVIRONMENT contains null values\n");
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
	key[i] = '\0';
	currentEnv->key = key;
	char *value =(char*)malloc(len-i);
	strncpy(value,(inputEnv+i+1),(len-i));
	currentEnv->value = value;
}


void initializeenv(char **inputEnv) {

	if(inputEnv == 0) {
		printf("envp is null \n");
		return ;
	}


	int i =0;
	i=0;
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
		parsecurrenv(currentEnv,inputEnv[i]);
		envSize++;
		i++;
	}
	environment = head;
	buildenv();
}

void getvalue(char* key, char *val) {
	if(key == 0) {
		printf("PATH HELPER.c:getValue : FATAL : Key passed is null\n");
	}
	envList *head =  environment;
	while(head != 0) {
		if(strcmp(head->key,key) == 0) {
			strcpy(val,head->value);
            return;
		}
		head = head->next;
	}
}

void setvalue(char* key, char *val) {
	if(key == 0) {
		printf("ENV HELPER.c: setValue: FATAL : Key passed is null\n");
	}
	if(val == 0) {
		printf("ENV HELPER.c: setValue: FATAL : val passed is null\n");
	}
	envList *head =  environment;
    int found = FALSE;
	while(head != 0) 
    {
		if(strcmp(head->key,key) == 0) {
			free(head->value);
		    found = TRUE;	
            head->value = (char *)malloc(strlen(val)+1);
            strcpy(head->value,val);
            break;
			//printf("%s\n",head->value);
		}
		head = head->next;
	}
	head = environment;
    if(!found)
    {
        while(head!=0 && head->next!=0) head++;
        if(head==0)
        {
            head = (envList*)malloc(sizeof(envList));
            head->key = (char*)malloc(sizeof(key)+1);
            strcpy(head->key,key);
            head->value = (char*)malloc(sizeof(val)+1);
            strcpy(head->value,val);
            head->next=0;
        }
        else
        {
            head->next = (envList*)malloc(sizeof(envList));
            head++;
            head->key = (char*)malloc(sizeof(key)+1);
            strcpy(head->key,key);
            head->value = (char*)malloc(sizeof(val)+1);
            strcpy(head->value,val);
            head->next=0;
        }
    }

	buildenv();

}

void buildenv() {
	envList *head =  environment;
	if(env != 0) {
		freeenv(env);
	}
	env = (char **)malloc(sizeof(char *)*(envSize+1));

	int i=0;
	while(head !=0) {
		int len = strlen(head->key) + strlen(head->value) + 2;
		char* envVar = (char *)malloc(len);
		memset(envVar, '\0',len);
		strcpy(envVar,head->key);
		strcat(envVar,"=");
		strcat(envVar,head->value);
		env [i] = envVar;
		head = head->next;
		i++;
	}

	env [i]= 0;

}

char *** getenv() {
	return &(env);
}

void freeenv(char **envlist){
	int i = 0;
	while(envlist[i]!= 0){
		free(envlist[i]);
		i++;
	}
	free(envlist);
}

char* appendpath(char* PATH,char* appendPATH){
	int newLength = strlen(PATH) + strlen(appendPATH) + 1;
	char *newPATH = (char *)malloc(sizeof(newLength)) ;
	strcpy(newPATH, PATH);
	strncpy(newPATH+strlen(PATH), appendPATH, strlen(appendPATH)+1);
	return newPATH;
}

char* prependpath(char PATH[],char prependPATH[]){
	int newLength = strlen(PATH) + strlen(prependPATH) + 1;
	char *newPATH;
	newPATH = (char *)malloc(newLength);
	strcpy(newPATH, prependPATH);
	strncpy(newPATH+strlen(prependPATH), PATH, strlen(PATH)+1);
	return newPATH;
}
