#ifndef _STDANDARDLIB_H_
#define _STANDARDLIB_H_

#include <stdarg.h>
#include <stdint.h>
//#include <stdio.h>
#include <syscalls.h>
#include <color.h>

void put_char(uint8_t fd, char c);
void sprint(uint8_t fd, char * str);
void my_printf(const char * frmt, ...);
char *convert(unsigned int num, int base);
//uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

#endif
