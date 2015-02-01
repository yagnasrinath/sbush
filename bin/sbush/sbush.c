#include<stdlib.h>
#include<stdio.h>
#include"sbush.h"
#include"bushUtils.h"
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
        trim(cmdline); 
    struct command* cm = (struct command*) malloc(sizeof(struct command));
    cm->executable = (char*)malloc(COMMAND_MAX_LENGTH);
    cm->argv = (char**)malloc(sizeof(char*)*MAX_COMMAND_ARGS);
    cm->next=0;
    cmd_list->start = cm;
    struct command* temp=cmd_list->start;
    int is_cmd=1,is_arg=0;//0 - False, 1 - True
    int cmd_index = 0, arg_val_index=0, arg_index = -1;
    while(*cmdline)
    {
        if(*cmdline=='|')
        {
            arg_index ++;
            temp->argv[arg_index] = 0;
            cm = (struct command*) malloc(sizeof(struct command));
            cm->executable = (char*)malloc(COMMAND_MAX_LENGTH);
            cm->argv = (char**)malloc(sizeof(char*)*MAX_COMMAND_ARGS);
            cm->next=0;
            temp->next = cm;
            temp = cm;
            is_cmd = 1;
            is_arg = 0;
            arg_val_index = 0;
            arg_index = -1;
        }
        else
        {
            if(*cmdline==' ')
            {
                is_cmd = 0;
                is_arg = 1;
                cmd_index = 0;
                arg_val_index=0;
                arg_index++;
//                printf("%d\n",arg_index);
                cm->argv[arg_index] = (char*)malloc(ARG_MAX_LENGTH);
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
        free(cmd->argv);
        cmd = cmd->next;
    }
}

int main(int argc, char* argv[], char* envp[])
{
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
    delete_job(&cmd_list);
}


