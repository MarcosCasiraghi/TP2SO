#ifndef _SYNCTEST_H
#define _SYNCTEST_H

#include <stdint.h>
#include <utilities.h>
#include <syscalls.h>
#include <util.h>
#include <string.h>

uint64_t test_sync(uint64_t argc, char *argv[]);
uint64_t my_process_inc(uint64_t argc, char *argv[]);
void slowInc(int64_t *p, int64_t inc);

#endif