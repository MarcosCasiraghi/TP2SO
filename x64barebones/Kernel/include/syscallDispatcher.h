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


void syscallDispatcher();

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
void schedulerDispatcher(char * name, void * func, uint64_t parametro,uint64_t flags);
//time
void int_86(char * buffer);

void int_87(uint8_t * address, char * buffer);

#endif
