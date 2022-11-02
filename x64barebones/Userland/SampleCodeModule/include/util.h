#ifndef _UTIL_H
#define _UTIL_H

#include <stdint.h>
#include <syscalls.h>
#include <standardlib.h>

#define MAJOR_WAIT 10000000

uint32_t GetUint();

uint32_t GetUniform(uint32_t max);

uint8_t memcheck(void *start, uint8_t value, uint32_t size);

void idleProcess();

void busyWait(int time);

#endif