#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

int add_task( void * task,int ground,int priority,uint64_t argc, char * argv[], uint64_t flags);
int tasksRunning(void);

//recive por parametro flag para saber que caso tiene que ejecutarse
//(matar a un programa activo, ambos, solo izq, solo derecha)
void schedulerExit(int amountOfFuncs);
int getActivePId();
int getPID();
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

int killProcess(int pid);
void nice(int pid, int priority);
int blockProcess(int pid);
int foregroundRunning();
uint64_t yield(uint64_t * registers, uint8_t load);

#endif
