/*
 * fs.h
 *
 *  Created on: Apr 28, 2015
 *      Author: srinath
 */

#ifndef INCLUDE_SYS_FS_FS_H_
#define INCLUDE_SYS_FS_FS_H_
#include<sys/defs.h>

#define MAXLEN 50
#define MAXCHILD 8

enum
{
	FILE_TYPE,
	DIRECTORY_TYPE,
}file_type;

typedef struct pipe_dev
{
	uint64_t* buffer;
	uint64_t write_pos;
	uint64_t read_pos;
	uint64_t size;
} pipe_dev_t;

typedef struct file
{
	char file_name[MAXLEN];
	uint64_t start;
	uint64_t end;
	uint64_t curr;
	uint64_t type;
	struct file* fchild[MAXCHILD];
} file_t;

file_t* root_node;


typedef struct file_descriptor
{
	file_t* filenode;
	uint64_t file_type;
	pipe_dev_t* pipenode;
} file_des_t;

#endif /* INCLUDE_SYS_FS_FS_H_ */
