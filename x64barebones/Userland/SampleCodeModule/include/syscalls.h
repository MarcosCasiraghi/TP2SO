#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

void sys_write(char * buffer, uint8_t fontColor, uint8_t backColor, int length);
void sys_exit();
void sys_clear();
void sys_exec();
void sys_read(char * buffer);
void sys_scheduler(char * name, void * func,int ground,int priority, uint64_t parametro);
int sys_registers(uint64_t * reg);
void sys_time(char * buffer);
void sys_getMem(uint8_t * address, char * buffer);

#endif
