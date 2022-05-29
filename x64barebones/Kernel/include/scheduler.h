#ifndef SCHEDULER_H
#define SCHEDULER_H
void add_task(char *name, void * task);
void scheduler(void);
int tasksRunning(void);
void * getTask(int pID);

#endif