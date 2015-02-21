#include <stdio.h>
#include <stdlib.h>
#include<errno.h>
int main(int argc,char** argv,char** envp){
    char buf[10240];
    int size;
    if(argc == 1){
        while(1){
            if((size = read(0,buf,10240))>0)
                write(1, buf, size);
            else
                break;
        }
    }
    else{
        int fd;
        argv++;
        while(argc > 0){
            fd = open(*argv,O_RDONLY);
            while(1){
                if((size = read(fd, buf,10240))>0){
                    printf("Size of file is %d\n\x1b[0m",size);
                    buf[size] = '\0';
                    write(1, buf, size);
                }
                else
                    break;
            }
            argc--;
            argv++;
        }
    }


}
