/*
 * process_helper.h
 *
 *  Created on: Apr 8, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_HELPER_H_
#define SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_HELPER_H_
#include<sys/defs.h>
#include <sys/ProcessManagement/process.h>
void remove_child(task_struct* child_task);
task_struct* create_new_task(BOOL is_user_process);
void add_free_task_struct (task_struct * new_task_struct) ;
vma_struct* create_new_vma(uint64_t file_desc, uint64_t vma_start, uint64_t vma_end, uint64_t vma_type, uint64_t vma_perm);
void initialize_free_list();
void free_task_struct(task_struct* to_free);
void detach_children(task_struct* parent_task_struct);
void detach_from_parent(task_struct* child_task_struct);
void free_process_vma_list(vma_struct* curr_list);
void decrement_pid();
int get_curr_pid();
task_struct* get_task_list();
#endif /* SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_HELPER_H_ */
