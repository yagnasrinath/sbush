#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<errno.h>

typedef struct
{
	int              fd;
	struct dirent    buffer ;
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
	return dir;
}


struct dirent* readdir(void* dir) {
	DIR* dirStruct  = (DIR *)dir;
	struct dirent * retDirEntry;

	int returnCount;
	returnCount = getdirentries(dirStruct->fd,&(dirStruct->buffer),sizeof(dirStruct->buffer));
	if(returnCount <= 0) {
		return NULL;
	}
	retDirEntry= (struct dirent*)&dirStruct->buffer;
	return retDirEntry;

}





int closedir(void *dir) {
	DIR* dirStruct  = (DIR *)dir;
	int returncode;
	returncode = close(dirStruct->fd);
	free(dirStruct);
	return returncode ;
}
