#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>
#include <color.h>

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncPrintwithAtt(const char * string, uint8_t fontColor, uint8_t backColor);
void ncPrintCharWithAtt(char character, uint8_t att);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
uint8_t* getPosition(uint16_t x,uint16_t y);
void printCharinPos(char character,uint16_t x, uint16_t y);
void printStrinPos(const char * string,uint16_t x,uint16_t y);
void backspace();
void blink(uint8_t backColor);
void scrollUp();
void restoreDefault();
#endif
