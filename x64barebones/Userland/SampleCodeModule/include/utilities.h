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
void kill(int argc, char ** argv);
char pipeRead(int pipeId);
int pipeWrite(int pipeId, char * string);
int pipeOpen(int pipeId);
int pipeClose(int pipeId);
void pipeStatus();
int semOpen(int id, int value);
int semClose(int id);
int post(int id);
int wait(int id);
void semStatus();
void mmStatus();
void yield();
void sleep(int secs);
void loop(int argc, char **argv);
void cat(int argc, char **argv);
void wc(int argc, char **argv);
void filter(int argc, char **argv);
void reader();
void writer();
void idleProcessShell();


#endif
