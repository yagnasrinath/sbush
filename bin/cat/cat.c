#include <stdio.h>
#include <stdlib.h>
#include<string.h>
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
		argc--;
		while(argc > 0){
			fd = open(*argv,O_RDONLY);
			if(fd < 0 ) {
				printf("%s ", strerror(errno));
				exit(0);
			}
			while(1){
				if((size = read(fd, buf,10240))>0){

					buf[size] = '\0';
					write(1, buf, size);

				}
				else if (size < 0) {
					printf("%s ",strerror(errno));
					write(1, "\n", 1);
					exit(0);
				}
				else
					break;
			}
			argc--;
			argv++;
		}
	}

	//write(1, "\n", 1);
}
