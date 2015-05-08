#include <stdio.h>
#include <stdlib.h>
#include<errno.h>
#include<string.h>

int main(int argc, char* argv[], char* envp[]) {
	/*printf("argv[0] is %p\n", argv[0]);
	printf("argv[0] is %s\n", argv[0]);
	printf("argv[1] is %p\n", argv[1]);
	//printf("argv[1] is %s\n", argv[1]);
	printf("envp[0] is %p\n", envp[0]);
	printf("envp[0] is %s\n", envp[0]);
	printf("envp[1] is %p\n", envp[1]);
	//	printf("envp[1] is %s\n", envp[1]);*/
	char buf[255];
	void* dir;
	if(argc == 1){
		getcwd(buf,255);
		dir = opendir(buf);
		if(!dir){
			printf("%s \n",strerror(errno));
			return 0;
		}
	}
	else {
		dir = opendir(argv[1]);
		if(!dir){
			printf("%s \n",strerror(errno));
			return 0;
		}
	}
	struct dirent* directoryentry = readdir(dir);
	if(!directoryentry){
		if(argv[1]) {
			printf("%s\n", argv[1]);
			return 0;
		}
		else {
			return 0;
		}

	}
	do{
		printf("%s\t",directoryentry->d_name);
		struct dirent* directoryentry = readdir(dir);
		if(!directoryentry){
			break;
		}

	}while(1);
	printf("\n");


	if( closedir(dir) < 0 ){
		//printf("Closing Directory Failed\n");
	}
	return 0;
}
