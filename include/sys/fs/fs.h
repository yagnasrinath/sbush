/*
 * fs.h
 *
 *  Created on: Apr 28, 2015
 *      Author: srinath
 */

#ifndef INCLUDE_SYS_FS_FS_H_
#define INCLUDE_SYS_FS_FS_H_
#include<sys/defs.h>




enum
{
	O_RDONLY = 0,
	O_WRONLY = 1,
	O_RDWR = 2,
	O_CREAT = 0x40,
	O_DIRECTORY = 0x10000
}file_flags;




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
	char file_name[MAX_FILE_NAME_LEN];
	uint64_t start;
	uint64_t end;
	uint64_t curr;
	uint64_t type;
	struct file* fchild[MAXCHILD];
} file_t;

file_t* root_node;


typedef struct file_descriptor
{
	file_t* file_ptr;
	uint64_t file_type;
	uint64_t curr;
	pipe_dev_t* pipenode;
	uint64_t file_perm;
} file_des_t;

#endif /* INCLUDE_SYS_FS_FS_H_ */
