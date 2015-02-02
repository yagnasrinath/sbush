/*
 * knowncommandexecutor.c
 *
 *  Created on: Feb 2, 2015
 *      Author: ravichandrasadineni
 */

#include "knowncommandexecutor.h"
static char* knowncommands[] = {"cd", "set"};
int knowncommandslength = 2;
static char prompt[128];
int isdirexist(char *newDirName){
	char *Exists;
	if((Exists = opendir(newDirName)) != 0)
		return TRUE;
	else{
		write(2,strerror(errno),sizeof(strerror(errno)));
	}
	return FALSE;
}

void setprompt(char* currpromt) {
	strncpy(prompt,currpromt,128);
}
void initprompt() {
	setprompt("sbush$");
}

char* getprompt() {
	return prompt;
}
void changedir(char *dirpath){
	char path[4096] ;
	trim(dirpath);
	if((dirpath == 0)||(strlen(dirpath) == 0)) {
		getvalue("HOME", path);
		if(strlen(dirpath) == 0) {
			free(dirpath);
		}
	}
	else {
		strncpy(path, dirpath, strlen(dirpath)+1);
		free(dirpath);
	}
	if(isdirexist(path) == TRUE){
		printf("Change DIR Successful \n");
		setvalue("PWD",path);
        memset(path,'\0',strlen(path));
        getvalue("PWD",path);
        printf("%s\n",path);
	}
}

int isknowncommand(char *command) {
	int  i;
	for(i=0; i<knowncommandslength; i++) {
		if(!strcmp(command,knowncommands[i])){
			return TRUE;
		}
	}
	return FALSE;
}

void executeknowncommand(char *command, char** args) {

	if(!strcmp(command,"cd")) {
		changedir(args[1]);
		return;
	}
	if(!strcmp(command,"set")) {
		if(!strcmp(args[1],"prompt")) {
			if(args[2] != 0) {
				strncpy(prompt,args[2],128);
				return;
			}
			return;
		}
	}
}
