#ifndef _SYSCALLDISPATCHER_H_
#define _SYSCALLDISPATCHER_H_

#include <stdint.h>
#include <naiveConsole.h>
#include <irqDispatcher.h>
#include <kb.h>
#include <scheduler.h>
#include <time.h>
#include <lib.h>
#include <interrupts.h>
#include <MemoryManager.h>
#include <semaphores.h>
#include <pipes.h>


uint64_t syscallDispatcher();

//write
void int_80(char * buffer, uint8_t fontColor, uint8_t backColor, int length);
//read
void int_81(char * buffer);
//clear
void int_82();
//scheduler exit
void int_83();

void registersForInforeg(uint64_t * registers);

//registers
int int_85(uint64_t * registers);
//add task para scheduler
int schedulerDispatcher(void * func,int ground, int priority,uint64_t argc, char * argv[],uint64_t flags);
//time
void int_86(char * buffer);

void int_87(uint8_t * address, char * buffer);

void * int_89(uint64_t size);

void int_90(void * address);

void int_91(char * buffer);

void int_92(char * buffer);

void int_94(int pid, int priority);

int int_97(int id, int value);

int int_98(int id);

int int_99(int id);

int int_100(int id);

void int_101(char * buffer);

int int_102(int pipeId);

int int_103(int pipeId);

int int_104(int pipeId);

int int_105(int pipeId, char * string);

void int_106(char * buffer);

int int_107();

#endif
