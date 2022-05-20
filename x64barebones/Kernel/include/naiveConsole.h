#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
uint8_t* getPosition(uint16_t x,uint16_t y);
void printCharinPos(char character,uint16_t x, uint16_t y);
void printStrinPos(const char * string,uint16_t x,uint16_t y);
#endif