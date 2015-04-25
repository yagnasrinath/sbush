/*
 * empty.c
 *
 *  Created on: Apr 14, 2015
 *      Author: ravichandrasadineni
 */

#include<stdio.h>
#include<stdlib.h>




int main(int argc, char* argv[], char* envp[]) {
	//int pid = -1;
	int i=0;
	for(i =0;i<2;i++){
		printf("in loop %d\n",i);
		fork();
			//printf("pid is %d\n",pid);

	}


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



