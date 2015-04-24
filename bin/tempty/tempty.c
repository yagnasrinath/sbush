/*
 * tempty.c
 *
 *  Created on: Apr 20, 2015
 *      Author: ravichandrasadineni
 */

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
	int a[9048];
	a[9047] = 1;
	printf("Value of a is %d\n",a[9047]);
	//while(1);
	return a[9047];
}
