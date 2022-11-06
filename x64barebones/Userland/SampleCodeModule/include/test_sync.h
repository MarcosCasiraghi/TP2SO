#ifndef _SYNCTEST_H
#define _SYNCTEST_H

#include <stdint.h>
#include <utilities.h>
#include <syscalls.h>
#include <util.h>
#include <string.h>
#include <shell.h>

void test_sync(int argc, char **argv);
void my_process_inc(uint64_t argc, char *argv[]);
void slowInc(int64_t *p, int64_t inc);

#endif