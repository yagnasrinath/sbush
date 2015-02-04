/*
 * knowncommandexecutor.h
 *
 *  Created on: Feb 2, 2015
 *      Author: ravichandrasadineni
 */

#ifndef KNOWNCOMMANDEXECUTOR_H
#define KNOWNCOMMANDEXECUTOR_H
#include<stdlib.h>
#include<stdio.h>
#include"sbushutils.h"
#include"sbconstants.h"
#include"envhelper.h"
#include"sberror.h"
#include"sblimits.h"
void setprompt(char* currpromt);
char* getprompt();
void initprompt();
int isknowncommand(char *command) ;
void executeknowncommand(char *command, char** args);
#endif /* SBUSH_BIN_SBUSH_KNOWNCOMMANDEXECUTOR_H_ */
