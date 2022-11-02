#include <pipes.h>
#include <semaphores.h>

#define MAX_PIPES 8

pipe pipes[MAX_PIPES];

int initialSemId = 100;

static int createPipe(int pipeId);
static int getPipeIndex(int pipeId);

int openPipe(int pipeId){
    int index = getPipeIndex(pipeId);
    if (index == -1){
        index = createPipe(pipeId);
        if (index == -1){
            return -1;
        }
    }
    pipes[index].processes++;
    return pipeId;
}

int writePipe(int pipeId, char * string){
    int index = getPipeIndex(pipeId);
    if (index == -1){
        return -1;
    }
    for (int i = 0; string[i] != '\0'; ++i) {
        wait(pipes[index].writeLock);

        if (pipes[index].writeIndex > BUFFERSIZE) {
            return -1;
        }
        pipes[index].buffer[pipes[index].writeIndex] = string[i];
        pipes[index].writeIndex++;

        post(pipes[index].readLock);
    }
    return pipeId;
}

int readPipe(int pipeId){
    int index = getPipeIndex(pipeId);
    if (index == -1){
        return -1;
    }

    wait(pipes[index].readLock);

    char c = pipes[index].buffer[pipes[index].readIndex];
    pipes[index].readIndex = (pipes[index].readIndex + 1) % BUFFERSIZE;  //Se lee el pipe de manera circular

    post(pipes[index].writeLock);

    return c;
}

int closePipe(int pipeId){
    int index = getPipeIndex(pipeId);
    if (index == -1){
        return -1;
    }

    pipes[index].processes--;

    if (pipes[index].processes > 0){
        return 1;
    }

    pipes[index].present = 0;
    semClose(pipes[index].writeLock);
    semClose(pipes[index].readLock);

    return 1;
}

static int getPipeIndex(int pipeId) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i].id == pipeId) {
            return i;
        }
    }
    return -1;
}


static int createPipe(int pipeId) {
    int index;

    for (int i = 0; i < MAX_PIPES; ++i) {
        if (pipes[i].present != 1){
            index = i;
        }
        else if (i == MAX_PIPES-1){
            return -1;
        }
    }

    pipes[index].id = pipeId;
    pipes[index].readIndex = 0;
    pipes[index].writeIndex = 0;
    pipes[index].processes = 0;

    pipes[index].readLock = semOpen(initialSemId++, 0);
    pipes[index].writeLock = semOpen(initialSemId++, BUFFERSIZE);

    if (pipes[index].readLock == -1 || pipes[index].writeLock == -1) {
        return -1;
    }

    return pipeId;
}




void pipeStatus() {
//    printf("\n\nActive Pipe Status\n\n"); //TODO: no imprimir en el back
//    for (int i = 0; i < MAX_PIPES; i++) {
//        t_pipe pipe = pipes[i];
//        if (pipe.state == IN_USE) {
//            printf("Pipe ID: %d\n", pipe.id);
//            printf("    Amount of attached processes: %d\n", pipe.totalProcesses);
//            printf("    Read semaphore: %d\n", pipe.readLock);
//            printf("    Write semaphore: %d\n", pipe.writeLock);
//            printf("    Pipe buffer content: ");
//            for (int i = pipe.readIndex; i != pipe.writeIndex;
//                 i = (i + 1) % PIPE_BUFFER_SIZE) {
//                putChar(pipe.buffer[i]);
//            }
//        }
//    }
//    printf("\n\n");
}