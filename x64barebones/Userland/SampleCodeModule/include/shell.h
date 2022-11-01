#ifndef SHELL_H
#define SHELL_H

#define BUFFER_LENGTH 50
#define FOREGROUND 0
#define BACKGROUND 1
#define HIGHEST 0
#define MEDIUM 1
#define LOWEST 2
#define SHELLPRIO 3

#include <fibonacci.h>
#include <utilities.h>
#include <color.h>
#include <stdint.h>
#include <help.h>
#include <primos.h>
#include <div0.h>
#include <printInfo.h>
#include <invalidOpCode.h>
#include <mmTest.h>
#include <processTest.h>

void checkPrintMem(char* func1,char * parameter1);
void run(char * buffer);
void shell();
void initShell();
int addFunctions(char * buffer);
int getFuncIndex(char * func, int * flag);
void clearBuffer();
void checkKill(char * func, char * parameter);
#endif
