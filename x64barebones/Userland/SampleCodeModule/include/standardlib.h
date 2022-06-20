#ifndef _STDANDARDLIB_H_
#define _STANDARDLIB_H_

#include <stdarg.h>
#include <stdint.h>
#include <syscalls.h>
#include <color.h>

void put_char(char c);
void sprint(char * str);
void my_printf(const char * frmt, ...);
char *convert(unsigned int num, int base);
int strcmp(char * str1, char * str2);
int strlen(char * string);
void scanf(char * buffer, int maxlength);
char getChar();

#endif
