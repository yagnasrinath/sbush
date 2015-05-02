/*
 * pipe.c
 *
 *  Created on: Apr 29, 2015
 *      Author: ravichandrasadineni
 */
#include<sys/fs/fs.h>
#include<sys/sbunix.h>
pipe_dev_t* root_pipe;



uint64_t is_pipe_available() {
	pipe_dev_t* aux_pipe = root_pipe;
	while(aux_pipe != NULL) {
		if(!aux_pipe->isUsed) {
			return TRUE;
		}
		aux_pipe = aux_pipe->next;
	}
	return FALSE;
}


uint64_t open_pipe(file_des_t * fd1, file_des_t * fd2) {

	pipe_dev_t* aux_pipe = root_pipe;
	while(aux_pipe != NULL) {
		if(!aux_pipe->isUsed) {
			fd1->pipenode = aux_pipe;
			fd1->file_perm = O_RDONLY;
			fd1->file_type = PIPE_TYPE;
			fd2->pipenode = aux_pipe;
			fd2->file_perm = O_WRONLY;
			fd2->file_type = PIPE_TYPE;
			aux_pipe->isUsed = TRUE;
			aux_pipe->readEndRefCount = 1;
			aux_pipe->writeEndRefCount = 1;
			return TRUE;
		}
		aux_pipe = aux_pipe->next;

	}
	return FALSE;
}


static inline size_t pipe_unread(pipe_dev_t * pipe) {
	if (pipe->read_pos == pipe->write_pos) {
		return 0;
	}
	if (pipe->read_pos > pipe->write_pos) {
		return (pipe->size - pipe->read_pos) + pipe->write_pos;
	} else {
		return (pipe->write_pos - pipe->read_pos);
	}
}


int pipe_size(file_des_t * node) {
	pipe_dev_t * pipe = node->pipenode;
	return pipe_unread(pipe);
}



static inline size_t pipe_available(pipe_dev_t * pipe) {
	if (pipe->read_pos == pipe->write_pos) {
		return pipe->size - 1;
	}

	if (pipe->read_pos > pipe->write_pos) {
		return pipe->read_pos - pipe->write_pos - 1;
	} else {
		return (pipe->size - pipe->write_pos) + pipe->read_pos - 1;
	}
}


int pipe_unsize(file_des_t  * node) {
	pipe_dev_t * pipe = node->pipenode;
	return pipe_available(pipe);
}

static inline void pipe_increment_read(pipe_dev_t * pipe) {
	pipe->read_pos++;
	if (pipe->read_pos == pipe->size) {
		pipe->read_pos = 0;
	}
}

static inline void pipe_increment_write(pipe_dev_t * pipe) {
	pipe->write_pos++;
	if (pipe->write_pos == pipe->size) {
		pipe->write_pos = 0;
	}
}

static inline void pipe_increment_write_by(pipe_dev_t * pipe, size_t amount) {
	pipe->write_pos = (pipe->write_pos + amount) % pipe->size;
}


uint64_t read_pipe(file_des_t  * node, uint64_t size, char *buffer) {

	if(node->file_type != PIPE_TYPE) {
		panic("not a pipe !!!" );
	}
	if(node->pipenode == NULL) {
		panic("READ PIPE : PIPE DEVICE IS NULL!!!" );
	}
	if(!(node->file_perm  == O_RDONLY)) {
		return -1;
	}

	pipe_dev_t* pipe = node->pipenode;

	uint64_t collected = 0;
	__asm__ __volatile__ ("sti");
	while (pipe_unread(pipe) > 0 && collected < size && !(pipe->writeEndRefCount ==0)) {
		buffer[collected] = pipe->buffer[pipe->read_pos];
		pipe_increment_read(pipe);
		collected++;
	}
	__asm__ __volatile__ ("cli");
	return collected;
}




uint64_t write_pipe(file_des_t  * node, uint64_t size, char *buffer) {
	if(node->file_type != PIPE_TYPE) {
		panic("not a pipe !!!" );
	}
	if(node->pipenode == NULL) {
		panic("READ PIPE : PIPE DEVICE IS NULL!!!" );
	}
	if(!(node->file_perm  == O_WRONLY)) {
		return -1;
	}
	pipe_dev_t* pipe = node->pipenode;
	uint64_t written = 0;
	__asm__ __volatile__ ("sti");
	while (pipe_available(pipe) > 0 && written < size && !(pipe->readEndRefCount ==0)) {
		__asm__ __volatile__ ("cli");
		pipe->buffer[pipe->write_pos] = buffer[written];
		pipe_increment_write(pipe);
		written++;
		__asm__ __volatile__ ("sti");
	}
	__asm__ __volatile__ ("cli");

	return written;
}



uint64_t close_pipe(file_des_t * fd1) {

	pipe_dev_t* aux_pipe = fd1->pipenode;
	if(aux_pipe  == NULL) {
		panic("panic something wrong NO pipe!!!");
	}
	if(fd1->file_perm == O_WRONLY) {
		aux_pipe->writeEndRefCount--;
		//kprintf("write ref count on closing is %d %d \n ", aux_pipe->writeEndRefCount, aux_pipe->id);
	}
	else if (fd1->file_perm == O_RDONLY) {
		aux_pipe->readEndRefCount--;
		//kprintf("read ref count on closing is %d %d \n ", aux_pipe->readEndRefCount, aux_pipe->id);
	}
	if(aux_pipe->writeEndRefCount == 0 && aux_pipe->readEndRefCount == 0) {
		kprintf("pipe marked free \n");
		aux_pipe->isUsed = FALSE;
	}
	return TRUE;
}
