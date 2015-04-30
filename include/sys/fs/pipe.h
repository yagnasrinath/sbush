/*
 * pipe.h
 *
 *  Created on: Apr 29, 2015
 *      Author: ravichandrasadineni
 */

#ifndef INCLUDE_SYS_FS_PIPE_H_
#define INCLUDE_SYS_FS_PIPE_H_
#include<sys/defs.h>

uint64_t is_pipe_available() ;
uint64_t open_pipe(file_des_t * fd1, file_des_t * fd2);
uint64_t read_pipe(file_des_t  * node, uint64_t size, char *buffer) ;
uint64_t write_pipe(file_des_t  * node, uint64_t size, char *buffer) ;
uint64_t close_pipe(file_des_t * fd1) ;
#endif /* INCLUDE_SYS_FS_PIPE_H_ */
