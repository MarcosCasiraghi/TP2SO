#ifndef TPARQUI_KB_H
#define TPARQUI_KB_H

#include <stdint.h>
#include <scheduler.h>
#include <lib.h>
#include <MemoryManager.h>
#include <pipes.h>
#include <semaphores.h>

extern uint8_t kbFlag();
int getKey();
char getKbBuffer();

#endif
