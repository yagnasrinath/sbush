/*
 * tarfs.h
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_FS_TARFS_H_
#define SBUSH_INCLUDE_SYS_FS_TARFS_H_

#include<sys/defs.h>
#include<sys/sbunix.h>
#include "../utils/kstring.h"

extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

struct posix_header_ustar {
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char checksum[8];
	char typeflag[1];
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
	char pad[12];
};

typedef struct posix_header_ustar HEADER;

char* get_file_data(char* filename);

#endif /* SBUSH_INCLUDE_SYS_FS_TARFS_H_ */
