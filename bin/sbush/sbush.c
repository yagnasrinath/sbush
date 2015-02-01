#include<stdlib.h>
#include<stdio.h>
#include"sbush.h"
#include"sbushutils.h"
#include"sbconstants.h"
#include"envhelper.h"

void getabsolutepath(struct job *newcommand, char *path){
    struct job *tempjob = (struct job*)malloc(sizeof(struct job));


}

int getCountOfChar(char* string, char character) {
    if(string ==0 )
        return 0;
    int len = strlen(string);
    int count =0;
    for(int i=0;i <len; i++) {
        count++;
    }
    return count;
}
int getfirstindex(char* string,char character) {
    if(string ==0) {
        return 0;
    }
    int i=0;
    while(string[i]) {
        if(string[i]==character) {
            return i;
        }
        i++;
    }
    return -1;
}

char** strtokenize(char* string, char character) {
    int numofstrings = getcountofchar(string,character) +1;
    char **output = (char**)malloc(sizeof(char*)*numofstrings);
    int index=0;
    int i=0,j=0;
    while(string) {
        j = getfirstindex(string,character);
        if(j==-1){
            char* currstring = (char*)(malloc(strlen(string)+1));
            strcpy(currstring,string);
            output[index] = currstring;
            return output;
        }
        char* currstring = (char*)(malloc(j)+1);
        strncpy(currstring,string,j);
        currstring[j] ='\0';
        string+=j;
        output[index] =currstring;
        index++;
    }
    return 0;
}


char* get_line()
{
    int fd = 0; // 0 is stdin
    char *buf = (char*)malloc(JOB_MAX_LENGTH);
    write(1,"prompt$",7);
    size_t bytes_read = read(fd,buf,JOB_MAX_LENGTH-1); // can read only 31999 bytes
    // last char reserved for '\0'
    buf[bytes_read] = '\0';
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

int make_job(struct job* cmd_list,char * cmdline)
{
    if(cmdline == 0)
    {
        return 0;
    }
    ltrim(cmdline); 
    struct command* cm = (struct command*) malloc(sizeof(struct command));
    cm->executable = (char*)malloc(COMMAND_MAX_LENGTH);
    memset(cm->executable,'\0',COMMAND_MAX_LENGTH);
    cm->argv = (char**)malloc(sizeof(char*)*MAX_COMMAND_ARGS);
    cm->next=0;
    cmd_list->start = cm;
    struct command* temp=cmd_list->start;
    int is_cmd=1,is_arg=0;//0 - False, 1 - True
    int cmd_index = 0, arg_val_index=0, arg_index = 0;
    while(*cmdline)
    {
        if(*cmdline=='|')
        {
            temp->executable[cmd_index]='\0';
            rtrim(temp->executable);
            arg_index ++;
            temp->argv[arg_index] = 0;
            temp->argv[0] = (char*)malloc(COMMAND_MAX_LENGTH);
            memset(temp->argv[0],'\0',COMMAND_MAX_LENGTH);
            strcpy(temp->argv[0],temp->executable);
            cm = (struct command*) malloc(sizeof(struct command));
            cm->executable = (char*)malloc(COMMAND_MAX_LENGTH);
            memset(cm->executable,'\0',COMMAND_MAX_LENGTH);
            cm->argv = (char**)malloc(sizeof(char*)*MAX_COMMAND_ARGS);
            cm->next=0;
            temp->next = cm;
            cmd_index=0;
            temp = cm;
            is_cmd = 1;
            is_arg = 0;
            arg_val_index = 0;
            arg_index = 0;
        }
        else
        {
            if(*cmdline==' ')
            {
                is_cmd = 0;
                is_arg = 1;
                arg_val_index=0;
                arg_index++;
                //                printf("%d\n",arg_index);
                cm->argv[arg_index] = (char*)malloc(ARG_MAX_LENGTH);
                memset(cm->argv[arg_index],'\0',ARG_MAX_LENGTH);
            }
            else if(is_cmd)
            {
                cm->executable[cmd_index] = *cmdline;
                cmd_index++;
            }
            else if(is_arg)
            {
                //                printf("\n%c\t%d\n",*cmdline,arg_val_index,arg_index);
                cm->argv[arg_index][arg_val_index] = *cmdline;
                arg_val_index++;
            }
        }
        cmdline++;
    }
    arg_index ++;
    temp->argv[arg_index] = 0;
    temp->executable[cmd_index]='\0';
    rtrim(temp->executable);
    if(!temp->argv[0])
    {
        temp->argv[0] = (char*)malloc(COMMAND_MAX_LENGTH);
        memset(temp->argv[0],'\0',COMMAND_MAX_LENGTH);
        strcpy(temp->argv[0],temp->executable);
    }
    return 0;
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

void delete_job(struct job* cmd_list)
{
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
        //free(cmd->argv);
        cmd = cmd->next;
    }
}

void execute_job(struct job* j,char**envp)
{
    int old[2] = {-1,-1},new[2]={-1,-1};
    struct command* c = j->start;
    int pid = -1;
    while(c)
    {
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
                    dup2(0,old[0]);
                }
                close(new[0]);
                dup2(1,new[1]);
                char* args[] = {"/bin/ls",0};
                printf("%s\t%s\n",c->executable,c->argv[0]);
                execve("/bin/ls",args,envp);
                msg="execve failed";
                printf("%s,%d\n",msg,errno);
                exit(EXIT_FAILURE);
                break;
            default:
                close(old[0]);
                close(old[1]);
                old[0]=new[0];
                old[1]=new[1];
                break;
        }
        c=c->next;
    }
}

int main(int argc, char* argv[], char* envp[])
{
    initializeenv(envp);
    char** new_envp = getenv();
    char * line;
    if (argc == 1)
    {
        line = get_line();
    }
    else
    {
        line = get_args_line(argc,argv);
    }
    printf("%s\n",line);
    struct job cmd_list;
    make_job(&cmd_list,line);
    print_job(&cmd_list);
    //delete_job(&cmd_list);
    execute_job(&cmd_list,new_envp);
}
