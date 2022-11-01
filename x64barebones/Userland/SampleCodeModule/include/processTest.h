#ifndef _PROCESS_TEST_H
#define _PROCESS_TEST_H

#include <syscalls.h>
#include <shell.h>
#include <util.h>
#include <standardlib.h>
#include <utilities.h>

#define MAJOR_WAIT 10000000

void processTest();

void idleProcess();

void busyWait(int time);

#endif