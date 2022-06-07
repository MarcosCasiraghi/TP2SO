#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <naiveConsole.h>
#include <scheduler.h>

void exceptionDispatcher(int exception,uint64_t * registers);
void exceptionCall(char * message, uint64_t * registers);


#endif
