#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

void sys_write(char * buffer, uint8_t fontColor, uint8_t backColor, int length, int fd);
void sys_exit();
void sys_clear();
void sys_read(int fd, char * buffer);
void sys_scheduler(char * name, void * func, uint64_t parametro);
void sys_registers(uint8_t fd);
void sys_time();

#endif
