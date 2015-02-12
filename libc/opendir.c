#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>

typedef struct
{
	int              fd;
	size_t           numOfItems;
	struct dirent*   next;
	struct dirent    buffer[15];
}DIR;

void *opendir(const char *path)
{
	DIR* dir = (DIR*)malloc(sizeof(DIR));
	if(!dir) {
		return dir;
	}
	dir->fd = open(path,0);
	if(dir->fd < 0)
	{
		free(dir);
		return NULL;
	}
	else
	{
		dir->numOfItems = 0;
		dir->next = NULL;
	}
	return NULL;
}


struct dirent* readDir(void* dir) {
	DIR* dirStruct  = (DIR *)dir;
	struct dirent * retDirEntry;

	if(dirStruct->numOfItems == 0) {
		int returnCount;
		while(1) {
			returnCount = getdirentries(dirStruct->fd,dirStruct->buffer,sizeof(dirStruct->buffer));
			if(returnCount > 0 || errno != EINTR)
				break;
		}
		if(returnCount < 0) {
			return NULL;
		}
		dirStruct->numOfItems  = returnCount;
		dirStruct->next = dirStruct->buffer;
	}

	retDirEntry= dirStruct->next ;
	dirStruct->next=(struct dirent*)((char*)retDirEntry +retDirEntry->d_reclen);
	dirStruct->numOfItems -= retDirEntry->d_reclen;

	return retDirEntry;
}


