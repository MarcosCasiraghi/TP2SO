#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

void sys_write(char * buffer, uint8_t fontColor, uint8_t backColor, int length);
void sys_write_decimal(int number);
void sys_clear();
void sys_read(int fd, char * buffer);

#endif
