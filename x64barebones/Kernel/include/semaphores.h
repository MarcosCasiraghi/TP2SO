#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H
#define MAX_PROCESSES 16
#define NULL ((void *)0)
#include <stdint.h>
#include <stdlib.h>
#include <lib.h>
#include <MemoryManager.h>
#include <scheduler.h>

typedef struct semaphore{
    int id;
    int value;
    int blockedProccesses[MAX_PROCESSES];
    int blockCounter;
    int openCounter;
    struct semaphore * tail;
    int mutex;
}semaphore;

int wait(int id);

int post(int id);

int semClose(int id);

int semOpen(int  id, int value);

void semStatus(char * buffer);


#endif