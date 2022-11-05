#ifndef LIBASM_H
#define LIBASM_H

#include <stdint.h>

char *cpuVendor(char *result);

void loadRegisters();

uint8_t getRTC(int n);

void callTimerTick();

#endif