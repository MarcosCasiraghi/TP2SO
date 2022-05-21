#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

void sys_write(char * buffer, uint8_t fontColor, uint8_t backColor);
void sys_clear();

#endif
