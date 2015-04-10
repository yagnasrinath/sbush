/*
 * process_helper.h
 *
 *  Created on: Apr 8, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_HELPER_H_
#define SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_HELPER_H_
#include<sys/defs.h>

task_struct* create_new_task(BOOL is_user_process);
void add_free_task_struct (task_struct * new_task_struct) ;

#endif /* SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_HELPER_H_ */
