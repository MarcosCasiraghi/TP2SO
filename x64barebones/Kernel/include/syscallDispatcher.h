#ifndef _SYSCALLDISPATCHER_H_
#define _SYSCALLDISPATCHER_H_

#include <stdint.h>
#include <naiveConsole.h>
#include <irqDispatcher.h>
#include <kb.h>
#include <scheduler.h>

void syscallDispatcher();
void int_80(char * buffer, uint8_t fontColor, uint8_t backColor, int length, int fd);
void int_81(int fd, char * buffer);
void int_82();
void int_83(int number);
void int_85(uint8_t fd, uint64_t * registers);
void schedulerDispatcher(char * name, void * func, uint64_t parametro,uint64_t flags);

#endif
