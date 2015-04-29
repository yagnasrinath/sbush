/*
 * empty.c
 *
 *  Created on: Apr 14, 2015
 *      Author: ravichandrasadineni
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>



int main(int argc, char* argv[], char* envp[]) {
	//for(int i=0; i < 15; i++ ) {
		char a[500];
		read(0,a,50);
		//strncpy(a,"hi",2);
		//fork();
		//strncpy(a,"hi",2);
		printf("passed line is %s \n",a);
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



