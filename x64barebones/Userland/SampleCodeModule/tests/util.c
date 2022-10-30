#include "util.h"

uint32_t GetUniform(uint32_t max){
    uint32_t u = GetUint();
    return (u + 1.0) * 2.328306435454494e-10 * max;
}

//Memory
uint8_t memcheck(void *start, uint8_t value, uint32_t size){
  uint8_t *p = (uint8_t *) start;
  uint32_t i;

  for (i = 0; i < size; i++, p++)
    if (*p != value)
      return 0;

  return 1;
}

