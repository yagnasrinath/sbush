#include<stdlib.h>
#include<stdio.h>
#include"sbush.h"
#include"sbushutils.h"
#include"sbconstants.h"
#include"envhelper.h"
#include"knowncommandexecutor.h"


envList * environment;
int envSize=0;
char** env=0;
int isfileinput = 0;

void setabsolutepath(char*newexec,struct command *newcommand, char *path){
    strcpy(newexec,path);
    strcat(newexec,"/");
    strcat(newexec, newcommand->executable);
}

void print_job(struct job*cmd_list)
{
    struct command *cmd;
    cmd = cmd_list->start;
    int i=0;
    while(cmd!=0)
    {
        printf("command:%s\n args:",cmd->executable);
        i=0;
        while(cmd->argv[i]!=0)
        {
            printf("%s\t",cmd->argv[i]);
            i++;
        }
        printf("\n");
        cmd = cmd->next;
    }
}

void free_job(struct job*cmd_list) {
    struct command *cmd;
    cmd = cmd_list->start;
    int i=0;
    while(cmd!=0)
    {
        free(cmd->executable);
        i=0;
        while(cmd->argv[i]!=0)
        {
            free(cmd->argv[i]);
            i++;
        }
        free(cmd->argv);
        struct command *prev_cmd;
        prev_cmd = cmd;
        cmd = cmd->next;
        free(prev_cmd);
    }
}

char* get_line()
{
    int fd = 0; // 0 is stdin
    char *buf = (char*)malloc(JOB_MAX_LENGTH);
    if(!isfileinput){
        write(1,getprompt(),strlen(getprompt()));
        write(1," ",strlen(" "));
    }
    char c[1]={'\0'};
    int index = 0;
    while(read(fd,c,1))
    {
        if(c[0]=='\n')
        {
            buf[index]='\0';
            //printf("%s\n",buf);
            break;
        }
        else
        {
            buf[index]=c[0];
        }
        index++;
    }
    if((index == 0)&&(isfileinput))
    {
        exit(0);
    }
    trim(buf);
    return buf;
}



char* get_args_line(int argc,char*argv[])
{
    char *buf = (char*)malloc(JOB_MAX_LENGTH);
    buf [JOB_MAX_LENGTH-1] = '\0';
    int index = 0,arr_index = 0;
    char * temp;
    for(arr_index = 1; arr_index<argc;arr_index++)
    {
        temp = argv[arr_index];
        while(*temp && index < JOB_MAX_LENGTH-2)
        {
            buf[index] = *temp;
            temp++;
            index++;
        }
        buf[index] = ' ';
        index++;
    }
    buf[index-1] = '\0';
    return buf;
}

void make_job(struct job* cmd_list,char * cmdline)
{
    if(cmdline == 0)
    {
        cmd_list->start =0;
        return;
    }
    trim(cmdline); 
    if((cmdline[0]=='#')&&(isfileinput))
    {
        cmd_list->start=0;
        return;
    }
    char **list =strtokenize(cmdline,'|');
    char **_list = list;
    int firstcommand = TRUE;
    struct command* temp=0;
    while(*list)
    {
        trim(*list);
        if(strlen(*list))
        {
            struct command* cm = (struct command*) malloc(sizeof(struct command));
            if(firstcommand)
            {
                cmd_list->start = cm;
                temp = cm;
                firstcommand = FALSE;
            }
            else
            {
                temp->next = cm;
                temp = cm;
            }
            cm->executable = (char*)malloc(COMMAND_MAX_LENGTH);
            memset(cm->executable,'\0',COMMAND_MAX_LENGTH);
            cm->argv = (char**)malloc(sizeof(char*)*MAX_COMMAND_ARGS);
            cm->next=0;
            char **cmd_args = strtokenize(*list,' ');
            char **_cmd_args = cmd_args;
            strcpy(cm->executable,*cmd_args);
            cm->argv[0] = (char*)malloc(ARG_MAX_LENGTH);
            memset(cm->argv[0],'\0',ARG_MAX_LENGTH);
            strcpy(cm->argv[0],*cmd_args);
            cmd_args++;
            int index = 1;
            while(*cmd_args)
            {
                trim(*cmd_args);
                if(strlen(*cmd_args))
                {
                    cm->argv[index] = (char*)malloc(ARG_MAX_LENGTH);
                    memset(cm->argv[index],'\0',ARG_MAX_LENGTH);
                    strcpy(cm->argv[index],*cmd_args);
                    index++;
                }
                cmd_args++;
            }
            cm->argv[index]=0;
            free(_cmd_args);
            //printf("%s\n",cm->executable);
            index=0;
            //while(cm->argv[index])
            //printf("%s\n",cm->argv[index++]);
        }
        list++;
    }
    free(_list);
    return;
}




void execute_command(struct command*c, char***envp_ptr)
{

    int indexofslash =  getfirstindex(c->executable,'/');
    if(indexofslash != -1)
    {
        execve(c->executable,c->argv,*envp_ptr);
        write(2,strerror(errno),strlen(strerror(errno)));
        write(2,"\n",strlen("\n"));
        return;
    }
    else
    {
        char path[MAX_PATH_LENGTH];
        getvalue("PATH",path);
        //printf("path:%s\n",path);
        char** paths = strtokenize(path,':');
        while(*paths)
        {
            if(c->argv[0])
                free(c->argv[0]);
            char *cmdpath = (char*)malloc(MAX_PATH_LENGTH);
            setabsolutepath(cmdpath,c,*paths);
            c->argv[0]=cmdpath;
            execve(cmdpath,c->argv,*envp_ptr);
            if (errno != ENOENT && errno != EACCES)
            {
            	printf("Enter the process 1 \n");
                write(2,strerror(errno),strlen(strerror(errno)));
                printf("Enter the process 1 \n");
                write(2,"\n",strlen("\n"));
                return;
            }
            paths++;
        }
        char * msg="command not found";
        write(2,msg,strlen(msg));
        write(2,"\n",strlen("\n"));
    }
}

void execute_job(struct job* j,char***envp_ptr)
{
    int old[2] = {-1,-1},new[2]={-1,-1};
    struct command* c = j->start;
    int pid = -1;
    int childs=0;
    while(c)
    {
        if(isknowncommand(c->executable))
        {
            executeknowncommand(c->executable,c->argv);

        }
        else {
            if(pipe(new)==-1)
            {
                char * msg="pipe creation failed";
                write(2,msg,strlen(msg));
                exit(EXIT_FAILURE);
            }
            switch(pid = fork())
            {
                case (-1):
                    ;
                    char * msg="fork failed";
                    write(2,msg,strlen(msg));
                    exit(EXIT_FAILURE);
                    break;
                case 0:
                    if(old[1]!=-1)
                    {
                        close(old[1]);
                        dup2(old[0],0);
                    }
                    close(new[0]);
                    if(c->next!=0)
                    {
                        dup2(new[1],1);
                    }
                    execute_command(c,envp_ptr);
                    exit(EXIT_FAILURE);
                    break;
                default:
                    childs++;
                    if(old[0]!=-1)
                    {
                        close(old[0]);
                        close(old[1]);
                    }
                    old[0]=new[0];
                    old[1]=new[1];
                    break;
            }
        }
        c=c->next;
    }
    if(old[0]!=-1)
    {
        close(old[0]);
        close(old[1]);
    }
    while(childs--)
    {
        waitpid(-1,0,0);
    }
}

int main(int argc, char* argv[], char* envp[])
{
    initprompt();

    initializeenv(envp);
    char*** new_envp_ptr = getenv();
    char * line;
    struct job cmd_list;
    /*
       if (argc != 1)
       {
       line = get_args_line(argc,argv);
       make_job(&cmd_list,line);
       free(line);
       execute_job(&cmd_list,new_envp_ptr);
       delete_job(&cmd_list);
       return 0;
       }
       */
    if(argc == 2)
    {
        int fd = open(argv[1],O_RDONLY);
        isfileinput =1;
        dup2(fd,0);
    }
    while(1)
    {
        line = get_line();
        if(!strlen(line))
        {
            free(line);
            
            continue;
        }
        make_job(&cmd_list,line);
        free(line);
        execute_job(&cmd_list,new_envp_ptr);
        free_job(&cmd_list);
    }
}
