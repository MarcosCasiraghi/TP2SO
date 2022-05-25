#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <syscalls.h>
#include <color.h>
#include <standardlib.h>
#define KEYBOARD_FD 1
#define SCREEN_FD 0


//armar getchar
//armar scanf
void clear();
void print(char * string,uint8_t fontColor, uint8_t backColor);
void printDec(int number);
char getChar();
void scanf(int fd, char * buffer, int maxlength);

#endif
