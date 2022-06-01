#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

void add_task(char *name, void * task, uint64_t flags);
void * scheduler(void);
int tasksRunning(void);
void * getTask(int pID);

#endif
