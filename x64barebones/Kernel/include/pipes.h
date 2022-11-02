//
// Created by Gayba on 11/1/2022.
//

#ifndef TP2SO_PIPES_H
#define TP2SO_PIPES_H

#define BUFFERSIZE 100
#define USED 1
#define EMPTY 0

typedef struct pipe {
    int present;
    int id;
    char buffer[BUFFERSIZE];
    int writeIndex, readIndex;
    int processes;
    int writeLock, readLock;
} pipe;

int writePipe(int pipeId, char *str);
int openPipe(int pipeId);
int closePipe(int pipeId);
int readPipe(int pipeId);
void pipeStatus();

#endif //TP2SO_PIPES_H
