#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <shell.h>

void sys_write(char * buffer, uint8_t fontColor, uint8_t backColor, int length);
void sys_exit();
void sys_clear();
void sys_exec();
void sys_read(char * buffer);
int sys_scheduler(void * func,int ground,int priority, uint64_t argc, char * argv[]);
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
void sys_yield();
int sys_semOpen(int id, int value);
int sys_semClose(int id);
int sys_semWait(int id);
int sys_semPost(int id);
void sys_semStatus(char * buffer);
int sys_unblock(int pid);
int sys_getseconds();
int sys_pipeOpen(int pipeId);
int sys_pipeClose(int pipeId);
char sys_pipeRead(int pipeId);
int sys_pipeWrite(int pipeId, char * buffer);
void sys_pipeStatus(char * buffer);


#endif
