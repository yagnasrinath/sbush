/*
 * knowncommandexecutor.c
 *
 *  Created on: Feb 2, 2015
 *      Author: ravichandrasadineni
 */

#include "knowncommandexecutor.h"
static char* knowncommands[] = {"cd", "set"};
int knowncommandslength = 2;
int isdirExist(char *newDirName){
    char *Exists;
    if((Exists = opendir(newDirName)) != 0)
        return TRUE;
    else{
        write(2,strerror(errno),sizeof(strerror(errno)));
    }
    return FALSE;
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
    if(isdirExist(path) == TRUE){
        printf("Change DIR Successful \n");
        setvalue("PWD",path);
    }
}

int isKnowncommand(char *command) {
	int  i;
	for(i=0; i<knowncommandslength; i++) {
		if(!strcmp(command,knowncommands)){
			return TRUE;
		}
	}
	return FALSE;
}

void executecommand(char *command, char** args) {
	int i;
	if(!strcmp(command,"cd")) {
		changedir(args[1]);
	}
}
