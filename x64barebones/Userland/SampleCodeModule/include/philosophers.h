#ifndef _PHILOSOPHERS_H
#define _PHILOSOPHERS_H

#include <shell.h>
#include <syscalls.h>
#include <utilities.h>
#include <standardlib.h>

#define INITIAL_PHILOS 5
#define MAX_PHILOS 10

#define FILO_SEM_ID 1234
#define MUTEX_SEM_ID 4321
#define PHYLO_SLEEP_SECONDS 1
#define SLEEP_SECONDS 2

void philoProblem(int argc, char ** argv);

#endif