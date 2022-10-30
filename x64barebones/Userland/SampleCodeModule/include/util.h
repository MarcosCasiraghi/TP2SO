#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>
#include "syscalls.h"

uint32_t GetUniform(uint32_t max);

uint8_t memcheck(void *start, uint8_t value, uint32_t size);
#endif