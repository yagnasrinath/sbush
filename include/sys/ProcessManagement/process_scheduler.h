/*
 * process_scheduler.h
 *
 *  Created on: Apr 9, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_SCHEDULER_H_
#define SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_SCHEDULER_H_
#include<sys/defs.h>
#include<sys/ProcessManagement/process.h>
void schedule_process(task_struct * new_task, uint64_t stk_top, uint64_t entry);
task_struct * get_next_ready_proc() ;
void free_exit_process();
task_struct*  get_curr_task() ;
void create_idle_proc() ;
task_struct* create_idle_proc2() ;
void create_idle_proc3() ;
task_struct*  create_init_proc();
void awake_sleeping_proc() ;
void initialize_proc_scheduler() ;
void add_to_task_list(task_struct * new_proc);
#endif /* SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_SCHEDULER_H_ */
