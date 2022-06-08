#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <syscalls.h>
#include <color.h>
#include <standardlib.h>

void clear();
void print(char * string,uint8_t fontColor, uint8_t backColor);
char getChar();
void scanf(char * buffer, int maxlength);
void exit();
void time();

#endif
