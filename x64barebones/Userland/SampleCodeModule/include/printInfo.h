#ifndef _PRINTINFO_H_
#define _PRINTINFO_H_

#include <syscalls.h>
#include <color.h>
#include <standardlib.h>
#include <utilities.h>


void inforeg();

//se encarga de verificar que el parametro pasado a 
//printmem sea adecuado y convierte a decimal la direccion 
//que queda en result
int readAddress(char* s, uint8_t** result);
void printMem();

//convierte una hexa a un char
char hexaChar(uint8_t value);


#endif