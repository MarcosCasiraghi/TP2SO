#ifndef _IRQDISPATCHER_H_
#define _IRQDISPATCHER_H_

#include <stdint.h>
#include <naiveConsole.h>
#include <kb.h>
#include <color.h>
#include <time.h>


void irqDispatcher(uint64_t irq);
void int_20();

#endif
