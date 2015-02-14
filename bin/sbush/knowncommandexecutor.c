/*
 * knowncommandexecutor.c
 *
 *  Created on: Feb 2, 2015
 *      Author: ravichandrasadineni
 */

#include "knowncommandexecutor.h"
static char* knowncommands[] = {"cd", "set" ,"setenv","echo"};
int knowncommandslength = 4;
static char prompt[128];
int isdirexist(char *newDirName){
	void *Exists;
	if((Exists = opendir(newDirName)) != 0)
        return TRUE;
	else{
		write(2,strerror(errno),strlen(strerror(errno)));
        write(2, "\n",strlen("\n"));
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
	}
	else {
		strncpy(path, dirpath, strlen(dirpath)+1);
	}
	if(isdirexist(path) == TRUE){
		if(!chdir(path)){
            //printf("Change DIR Successful \n");
        }
        else {
            printf("%s\n",strerror(errno));
            //write(2,strerror(errno),strlen(strerror(errno)));
            //write(2,"\n",strlen("\n"));
        }
        getcwd(path,4096);
        setvalue("PWD",path);
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

    if(!strcmp(command,"cd")) 
    {
        changedir(args[1]);
        return;
    }
    
    if(!strcmp(command,"setenv"))
    {
        char** arg2tokens = strtokenize(args[2],':');
        int index=0;
         char *temp = 0;
        int size =0;
        for(index=0;arg2tokens[index]!=0;index++)
        {
            temp = strstr(arg2tokens[index],"${");
            if(temp!=0)
            {
                temp=temp+2;
                int pos=-1;
                if((pos=getfirstindex(temp,'}'))!=-1)
                {
                    char* key = (char*)malloc(pos+1);
                    strncpy(key,temp,pos);
                    key[pos]='\0';
                    char* value = (char*)malloc(MAX_PATH_LENGTH);
                    memset(value,'\0',MAX_PATH_LENGTH);
                    getvalue(key,value);
                    if(strlen(value)!=0)
                    {
                        free(arg2tokens[index]);
                        free(key);
                        arg2tokens[index] = value;
                    }
                    else
                    {
                        char * msg=":undefined variable.\n";
                        write(2,key,strlen(key));
                        write(2,msg,strlen(msg));
                        free(key);
                        free(value);
                        return;
                    }

                }
                else
                {
                    char * msg="Missing }.\n";
                    write(2,msg,strlen(msg));
                    return;
                }
            }
            size = size + strlen(arg2tokens[index])+1;
        }
        size += 2;
        char * value = (char*)malloc(size);
        temp = value;
        for(index =0;arg2tokens[index]!=0;index++)
        {
            strncpy(temp,arg2tokens[index],strlen(arg2tokens[index]));
            temp+=strlen(arg2tokens[index])+1;
            *temp=':';
        }
        *temp='\0';
        setvalue(args[1],value);
    }
    else if(!strcmp(command,"set")) 
    {
        printf("%s\n",args[1]);
        char* temp = strstr(args[1],"prompt=");
        if(temp!=0)
            trim(temp=temp+7);
        if(temp!=0 ) 
        {
            int pos = getfirstindex(temp,'\'');
            if(pos==0)
            {
                temp++;
                pos = getfirstindex(temp,'\'');
                if(pos!=-1)
                {
                    strncpy(prompt,temp,pos);
                    prompt[pos]='\0';
                    return;
                }
            }
        }
        char * msg="invalid usage\n";
        write(2,msg,strlen(msg));
        return;
    }
    else if(!strcmp(command,"echo")) 
    {
        trim(args[1]);
        char* value = (char*)malloc(MAX_PATH_LENGTH);
        memset(value,'\0',MAX_PATH_LENGTH);
        getvalue(args[1]+1,value);
        printf("%s\n",value);
        free(value);
    }
}
