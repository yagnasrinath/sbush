/*
 * fs.c
 *
 *  Created on: Apr 28, 2015
 *      Author: srinath
 */

#include<sys/fs/fs.h>
#include<sys/fs/tarfs.h>
#include<sys/defs.h>
#include<sys/MemoryManagement/kmalloc.h>
#include<sys/utils/kstring.h>
#include <sys/sys_generic_util.h>

void create_node(file_t * node,file_t* parent,char* name,uint64_t start,uint64_t end,uint64_t type)
{
	node->start=start;
	node->end=end;
	node->curr=start;
	node->fchild[0]=node;
	node->fchild[1]=parent;
	node->type=type;
	kstrcpy(node->file_name,name);
}

void parse(char* inp_path,uint64_t file_type,uint64_t start,uint64_t end)
{
	file_t* temp_node = NULL;
	file_t* curr_node = root_node;
	char* path = (char*)kmalloc(sizeof(char)*kstrlen(inp_path));
	kstrcpy(path,inp_path);
	char* temp = kstrtok(path,"/");
	int i=0;
	while(temp!=NULL)
	{
		file_t* aux_node = curr_node;
		for(i=2;i<curr_node->end;i++)
		{
			if(kstrcmp(temp,curr_node->fchild[i]->file_name)==0)
			{
				curr_node=(file_t*) curr_node->fchild[i];
				break;
			}
		}

		if(i==aux_node->end)
		{
			//void* ptr=(void*);
			temp_node = (file_t*)kmalloc(sizeof(file_t));
			create_node(temp_node,curr_node,temp,start,end,file_type);
			curr_node->fchild[curr_node->end]=temp_node;
			curr_node->end+=1;
			curr_node=temp_node;
		}
		temp=kstrtok(NULL,"/");
	}
}

void* init_tarfs()
{
	HEADER *header = (HEADER*) &_binary_tarfs_start;
	root_node = (file_t*)kmalloc(sizeof(file_t));
	//kprintf("root node is %p \n", root_node);
	create_node(root_node,root_node,"/",0,2,DIRECTORY_TYPE);
	uint64_t file_size =0;
	do
	{

		if(kstrcmp(header->typeflag,"5")==0)
		{
			//kprintf("size in string is %s \n", header->size);
			file_size = oct_to_dec(atoi(header->size));
			parse(header->name,DIRECTORY_TYPE,0,2);
			//kprintf("file_size  is %d\n", file_size);
			//kprintf("parsed first file %s\n", header->name);

		}
		else
		{
			//kprintf("size in string is %s , %d\n", header->size, atoi(header->size));
			file_size = oct_to_dec(atoi(header->size));
			//kprintf("file_size  is %d\n", file_size);
			parse(header->name,FILE_TYPE,(uint64_t)header+512,(uint64_t)header+512+file_size);
			//kprintf("parsed first file %s\n", header->name);
		}

		file_size = file_size + sizeof(HEADER);
		if(file_size%512) {
			file_size = ((file_size/512)*512)+512;
		}
		//kprintf("file size is %d",file_size);
		header = (HEADER* )((char *)header + file_size);

	}while(header<(HEADER*)&_binary_tarfs_end);
	//while(1);
	return root_node;
}
