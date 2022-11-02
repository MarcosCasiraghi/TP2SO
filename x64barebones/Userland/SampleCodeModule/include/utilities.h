#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <syscalls.h>
#include <color.h>
#include <standardlib.h>

void clear();
void print(char * string,uint8_t fontColor, uint8_t backColor);


void exit();
void time();
void ps();
void clearConsole();
void kill(char * pid);
int pipeRead(int pipeId);
int pipeWrite(int pipeId, char * string);
int pipeOpen(int pipeId);
int pipeClose(int pipeId);
int pipeStatus(int pipeId, char * buffer);


#endif
