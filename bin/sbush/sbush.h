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
int getcountofchar(char* string, char character);
int getfirstindex(char* string,char character);
char** strtokenize(char* string, char character);
