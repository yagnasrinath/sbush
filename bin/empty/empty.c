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

	/*//for(int i=0; i < 15; i++ ) {
	char a[500];
	char b[500];
	strcpy(b, "this is a test for stack");
	strcpy(a, "this is a test for stack");
	printf(" entered stuff is %s\n", b);
	int p[2];
	int fd = pipe(p);
	printf(" fd, pipe1,pipe2 is %d,%d,%d \n", fd,p[0],p[1]);
	printf(" fd, pipe1,pipe2 is %d \n", p[1]);
	int ret = write(p[1],a,20);
	printf(" ret  is %d \n", ret);
	printf(" entered stuff is %s \n", a);
	if(fork() ==0) {
		printf(" before reading \n");
		int ret = read(p[0],b,22);
		printf(" entered stuff is %s, %d\n", b,ret);
	}*/
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
	int* b[10];
	for(int i=0 ; i<1; i+=2) {
		b[i] = (int*)malloc(sizeof(int));
		b[i+1] = (int*)malloc(sizeof(char)*17);
	}
	for(int i=0 ; i<1; i+=2) {
			free(b[i]);
			free(b[i+1]);
		}
	while(1);

	return 0;
}



