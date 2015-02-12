/*
 * getdirectoryentries.c
 *
 *  Created on: Feb 12, 2015
 *      Author: ravichandrasadineni
 */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<syscall.h>

int getdirentries(int fd, struct dirent* direntry , int size )
{
	syscall_3((uint64_t)SYS_getdents, (uint64_t) fd, (uint64_t) direntry, (uint64_t) size);

    printf("%d\n",errno);
    return 0;
}


