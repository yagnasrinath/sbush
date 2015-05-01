#include <stdio.h>
#include <stdlib.h>
#include<errno.h>

int main(int argc, char* argv[], char* envp[]) {
	char buf[255];
    void* dir; 
    if(argc == 1){
        getcwd(buf,255);

        dir = opendir(buf);
        if(!dir){
            printf("Opening Directory failed\n");
            return 0;
        }
    }
    else {
        dir = opendir(argv[1]);
        if(!dir){
            printf("Opening Directory failed\n");
            return 0;
        }
    }
    while(1){
        struct dirent* directoryentry = readdir(dir);
        if(!directoryentry){
            break;
        }
        printf("%s\t",directoryentry->d_name);
    }
    printf("\n");


    if( closedir(dir) < 0 ){
        //printf("Closing Directory Failed\n");
    }
    return 0;
}
