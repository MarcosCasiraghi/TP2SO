#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char hexaChar(uint8_t value);

void mutex_lock(int *mutex);
void mutex_unlock(int * mutex);
int _xchg();

#endif