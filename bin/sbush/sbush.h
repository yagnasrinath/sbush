#include"sblimits.h"

struct command
{
    char* executable;
    char** argv; //last argument is null
    struct command* next;
};

struct job
{
    struct command* start;
    struct job* next;
};

int make_job(struct job* cmd_list,char * cmdline);
