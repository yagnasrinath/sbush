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
<<<<<<< HEAD
=======
int getcountofchar(char* string, char character);
int getfirstindex(char* string,char character);
char** strtokenize(char* string, char character);
>>>>>>> 8feccc6f0c898c731bc550600f1442f77e0ccc15
