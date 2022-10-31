#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

void add_task(char *name, void * task,int ground,int priority,uint64_t parametro, uint64_t flags);
int tasksRunning(void);

//recive por parametro flag para saber que caso tiene que ejecutarse
//(matar a un programa activo, ambos, solo izq, solo derecha)
void schedulerExit(int amountOfFuncs);
int getParameter();
int getActivePId();
int shellRunning();
uint64_t * registerManager(uint64_t * registers, uint8_t load);
int getProcesses();
int getSplitScreen();
int tasksReady();

//a partir de los programas activos, alterna el ActivePID
void next();
int isForeground();
void ps(char * buffer);
void exitCurrent();

void killProcess(int pid);
void nice(int pid, int priority);
void blockProcess(int pid);

#endif
