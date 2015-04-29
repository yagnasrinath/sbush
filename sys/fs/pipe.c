/*
 * pipe.c
 *
 *  Created on: Apr 29, 2015
 *      Author: ravichandrasadineni
 */
#include<sys/fs/fs.h>
#include<sys/sbunix.h>
pipe_dev_t* root_pipe;

int open_pipe(file_des_t * fd1, file_des_t * fd2) {

	pipe_dev_t* aux_pipe = root_pipe;
	while(aux_pipe != NULL) {
		if(!aux_pipe->isUsed) {
			fd1->pipenode = aux_pipe;
			fd1->file_perm = O_RDONLY;
			fd1->file_type = PIPE_TYPE;
			fd1->pipenode = aux_pipe;
			fd1->file_perm = O_WRONLY;
			fd1->file_type = PIPE_TYPE;
			aux_pipe->isUsed = TRUE;
			return TRUE;
		}
		aux_pipe = aux_pipe->next;

	}
	return FALSE;
}




int close_pipe(file_des_t * fd1) {

	pipe_dev_t* aux_pipe = fd1->pipenode;
	if(aux_pipe  == NULL) {
		panic("panic something wrong NO pipe!!!");
	}
	aux_pipe->isUsed = FALSE;
	return TRUE;
}
