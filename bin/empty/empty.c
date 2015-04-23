/*
 * empty.c
 *
 *  Created on: Apr 14, 2015
 *      Author: ravichandrasadineni
 */

#include<stdio.h>
#include<stdlib.h>




int main(int argc, char* argv[], char* envp[]) {
	int pid;
	pid = fork();
	if(pid == 0) {
		printf("In child process \n");
		int *b ;
		b = (int*)malloc(sizeof(int));
		*b = 3;
		printf("In child process %d\n",*b);
	}
	else {
		printf("Pid of parent process is %d \n", pid);
		//int *b = 4;
		//printf("In parent process %d\n",b);
	}
	while(1);

	return 0;
}



