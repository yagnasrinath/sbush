/*
 * elf.h
 *
 *  Created on: Apr 10, 2015
 *      Author: ravichandrasadineni
 */

// taken from http://man7.org/linux/man-pages/man5/elf.5.html

#ifndef SBUSH_INCLUDE_SYS_ELF_LOADER_H_
#define SBUSH_INCLUDE_SYS_ELF_LOADER_H_

#include<sys/ProcessManagement/process.h>

#define EI_NIDENT 16
typedef  uint64_t Elf64_Off;
typedef  uint64_t Elf64_Addr;


typedef struct {
	uint32_t   p_type;
	uint32_t   p_flags;
	Elf64_Off  p_offset;
	Elf64_Addr p_vaddr;
	Elf64_Addr p_paddr;
	uint64_t   p_filesz;
	uint64_t   p_memsz;
	uint64_t   p_align;
} Elf64_Phdr;

typedef struct {
	uint32_t   sh_name;
	uint32_t   sh_type;
	uint64_t   sh_flags;
	Elf64_Addr sh_addr;
	Elf64_Off  sh_offset;
	uint64_t   sh_size;
	uint32_t   sh_link;
	uint32_t   sh_info;
	uint64_t   sh_addralign;
	uint64_t   sh_entsize;
} Elf64_Shdr;

typedef struct {
	unsigned char e_ident[EI_NIDENT];
	uint16_t      e_type;
	uint16_t      e_machine;
	uint32_t      e_version;
	Elf64_Addr     e_entry;
	Elf64_Off      e_phoff;
	Elf64_Off      e_shoff;
	uint32_t      e_flags;
	uint16_t      e_ehsize;
	uint16_t      e_phentsize;
	uint16_t      e_phnum;
	uint16_t      e_shentsize;
	uint16_t      e_shnum;
	uint16_t      e_shstrndx;
} Elf64_Ehdr;
task_struct * get_elf_task(char *filename, char *argv[], char* env[]);

#endif /* SBUSH_INCLUDE_SYS_ELF_LOADER_H_ */
