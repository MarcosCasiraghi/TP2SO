#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

void sys_write(char * buffer, uint8_t fontColor, uint8_t backColor, int length);
void sys_exit();
void sys_clear();
void sys_exec();
void sys_read(char * buffer);
int sys_scheduler(char * name, void * func,int ground,int priority, uint64_t parametro);
int sys_kill( int pid);
void sys_nice( int pid, int priority);
int sys_block( int pid );
int sys_registers(uint64_t * reg);
void sys_time(char * buffer);
void sys_getMem(uint8_t * address, char * buffer);
void * sys_malloc(uint64_t size);
void sys_free(void * address);
void sys_memStatus(char * buffer);
void sys_ps(char* buffer);
int sys_getPID();

#endif
