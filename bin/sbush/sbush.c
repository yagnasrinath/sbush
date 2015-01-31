#include<stdlib.h>
#include<stdio.h>

char* get_line()
{
    int fd = 0; // 0 is stdin
    int max = 32000;
    char *buf = (char*)malloc(max);
    size_t bytes_read = read(fd,buf,max-1); // can read only 31999 bytes
                                            // last char reserved for '\0'
    buf[bytes_read] = '\0';
    return buf;
}

char* get_args_line(int argc,char*argv[])
{
    int max = 32000;
    char *buf = (char*)malloc(max);
    buf [max-1] = '\0';
    int index = 0,arr_index = 0;
    char * temp;
    for(arr_index = 1; arr_index<argc;arr_index++)
    {
        temp = argv[arr_index];
        while(*temp && index < max-2)
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

int main(int argc, char* argv[], char* envp[])
{
    char * line;
    if (argc == 1)
    {
        line = get_line();
        set_prompt("")
    }
    else
    {
        line = get_args_line(argc,argv);
    }
    printf("%s\n",line);
}


