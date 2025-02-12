#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <irqDispatcher.h>
#include <interrupts.h>
#include <libasm.h>

int add_task( void * task,int ground,int priority,uint64_t argc, char * argv[], uint64_t flags);

//recive por parametro flag para saber que caso tiene que ejecutarse
//(matar a un programa activo, ambos, solo izq, solo derecha)
void schedulerExit();
int getPID();
int shellRunning();
uint64_t * registerManager(uint64_t * registers, uint8_t load);
int getProcesses();

//a partir de los programas activos, alterna el ActivePID
void next();
int isForeground();
void ps(char * buffer);
void exitCurrent();

int killProcess(int pid);
void nice(int pid, int priority);
int blockProcess(int pid);
int unblockProcess(int pid);
int foregroundRunning();
int foregroundRunning2();
int blockProcessTick(int pid);
uint64_t * yield(uint64_t * registers, uint8_t load);


#endif
