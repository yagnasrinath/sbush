/*
 * empty.c
 *
 *  Created on: Apr 14, 2015
 *      Author: ravichandrasadineni
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/defs.h>


int main(int argc, char* argv[], char* envp[]) {

	//for(int i=0; i < 15; i++ ) {
	char a[500];
	int fd = open("/bin/ls",3);
	printf(" fd is %d \n", fd);

	read(fd,a,1);
	printf(" entered stuff is %s \n", a);
	if(fork() ==0) {
		int ret = read(fd,a,3);
			printf(" entered stuff is %s %d,\n", a,ret);
	}
	//strncpy(a,"hi",2);
	//int pid = fork();
	//strncpy(a,"hi",2);
	//printf("passed line is  %d \n",pid);
	//void* dirp =opendir("/bin");
	//struct dirent *dp;
	//while ((dp = readdir(dirp)) != NULL) {

	//}

	//}



	//while(1);
	//}

	//	if(pid == 0) {
	//		printf("In child process \n");
	//		int *b ;
	//		b = (int*)malloc(sizeof(int));
	//		 *b = 3;
	//		printf("In child process %d\n",*b);
	//		//sleep(3);
	//		//printf("In child process after sleep \n");
	//		//printf("In child process %d\n",getpid());
	//
	//		//printf("In child process %d\n",getppid());
	//	}
	//	else {
	//		//printf("Pid of parent process is %d \n", pid);
	//		//printf("In parent process %d\n",getpid());
	//		//printf("In parent process %d\n",getppid());
	//		int wait = waitpid(pid,0,0);
	//		printf("The value returned by waitpid is %d\n",wait);
	//
	//		//int *b = 4;
	//
	//		//printf("In parent process %d\n",b);
	//	}

	return 0;
}



