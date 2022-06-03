#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

void add_task(char *name, void * task,uint64_t parametro, uint64_t flags);
int tasksRunning(void);
void * getTask(int pID);
void schedulerExit(int amountOfFuncs);
<<<<<<< HEAD
int getParameter();
int getActivePId();
=======
int shellRunning();
>>>>>>> f6f06152394013917b1c724cc84fececead78c71

#endif
