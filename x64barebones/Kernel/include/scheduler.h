#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>

void add_task(char *name, void * task,uint64_t parametro, uint64_t flags);
int tasksRunning(void);
void * getTask(int pID);
void schedulerExit(int amountOfFuncs);
int getParameter();
int getActivePId();
int shellRunning();

#endif
