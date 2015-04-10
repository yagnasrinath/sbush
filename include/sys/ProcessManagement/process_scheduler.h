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

#endif /* SBUSH_INCLUDE_SYS_PROCESSMANAGEMENT_PROCESS_SCHEDULER_H_ */
