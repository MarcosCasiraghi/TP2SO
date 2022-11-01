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
int semOpen(int id, int value);
int semClose(int id);
int post(int id);
int wait(int id);
void semStatus();

#endif
