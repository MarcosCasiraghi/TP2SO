#ifndef SHELL_H
#define SHELL_H

#define BUFFER_LENGTH 50

#include <fibonacci.h>
#include <utilities.h>
#include <color.h>
#include <stdint.h>
#include <help.h>
#include <primos.h>

int run(char * buffer);
void shell();
void initShell();
#endif