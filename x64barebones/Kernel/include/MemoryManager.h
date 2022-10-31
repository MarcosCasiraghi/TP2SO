#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <naiveConsole.h>
void freeAll();
void initializeMemoryManager( char * startAddress, size_t size);

void allocMemory( const uint64_t nbytes, void ** addressToRet);

void freeMemory( void * block);

void memoryDump(char * buffer);

#endif