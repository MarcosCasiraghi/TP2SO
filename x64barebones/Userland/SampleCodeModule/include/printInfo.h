#ifndef _PRINTINFO_H_
#define _PRINTINFO_H_

#include <syscalls.h>
#include <color.h>
#include <standardlib.h>
#include <utilities.h>


void inforeg();
int readAddress(char* s, uint8_t** result);
void printMem();
char hexaChar(uint8_t value);


#endif