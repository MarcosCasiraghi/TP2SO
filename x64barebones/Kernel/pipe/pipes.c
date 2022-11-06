#include <pipes.h>
#include <semaphores.h>

#define MAX_PIPES 8

pipe pipes[MAX_PIPES];

static int initialSemId = 100;


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
        waitBack(pipes[index].writeLock);


        if (pipes[index].writeIndex > BUFFERSIZE) {
            return -1;
        }
        pipes[index].buffer[pipes[index].writeIndex] = string[i];
        pipes[index].writeIndex = (pipes[index].writeIndex + 1) % BUFFERSIZE;

        post(pipes[index].readLock);

    }

    return pipeId;
}

char readPipe(int pipeId){
    int index = getPipeIndex(pipeId);
    if (index == -1){
        return '\t';
    }

    waitBack(pipes[index].readLock);
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

void closeAllPipes(){
    for (int i = 0 ; i < MAX_PIPES ; i++){
        if(pipes[i].present == 1){
            pipes[i].present =0;
            semClose(pipes[i].writeLock);
            semClose(pipes[i].readLock);
        }
    }
}


static int getPipeIndex(int pipeId) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i].id == pipeId && pipes[i].present == 1) {
            return i;
        }
    }
    return -1;
}


static int createPipe(int pipeId) {
    int index;
    int notFound = 0;

    for (int i = 0; i < MAX_PIPES && !notFound; ++i) {
        if (pipes[i].present != 1){
            index = i;
            notFound = 1;
        }
        else if (i == MAX_PIPES-1){
            return -1;
        }
    }

    pipes[index].id = pipeId;
    pipes[index].readIndex = 0;
    pipes[index].writeIndex = 0;
    pipes[index].processes = 0;
    pipes[index].present = 1;
    pipes[index].buffer[0] = '\0';

    pipes[index].readLock = semOpen(initialSemId++, 0);
    pipes[index].writeLock = semOpen(initialSemId++, BUFFERSIZE);

    if (pipes[index].readLock == -1 || pipes[index].writeLock == -1) {
        return -1;
    }

    return pipeId;
}



void pipeStatus(char * buffer) {
    char title[] = {"Active Pipes:\n\n"};
    int i;
    for(i = 0; title[i]!= '\0' ; i++) {
        buffer[i] = title[i];
    }

    int k = 0;
    int activeFlag = 0;

    // i = 0;

    while(k < MAX_PIPES){
        if (pipes[k].present == 1){
            activeFlag = 1;
            char idString[] = {"ID: "};
            for(int j = 0; idString[j]!= '\0' ; i++,j++){
                buffer[i] = idString[j];
            }

            char id[5] = {0};
            uintToBase(pipes[k].id, id, 10);
            for(int j = 0; id[j]!= '\0' ; i++,j++){
                buffer[i] = id[j];
            }

            char stateString[] = {"\nState: "};
            for(int j = 0; stateString[j]!= '\0' ; i++,j++){
                buffer[i] = stateString[j];
            }

            char * state;

            if (pipes[i].writeIndex == 0 && pipes[i].readIndex == 0){
                state = "Empty";
            }
            else{
                state = "In use";
            }

            for(int j = 0; state[j]!= '\0' ; i++,j++){
                buffer[i] = state[j];
            }

            char amountString[] = {"\nAmount of Processes Involved: "};
            for(int j = 0; amountString[j]!= '\0' ; i++,j++){
                buffer[i] = amountString[j];
            }

            char amount[5] = {0};
            uintToBase(pipes[k].processes, amount, 10);
            for(int j = 0; amount[j]!= '\0' ; i++,j++){
                buffer[i] = amount[j];
            }

            char newLine[] = {"\n"};
            buffer[i++]=newLine[0];
        }
        k++;
    }

    if (activeFlag == 0){
        char message[] = {"No active pipes\n"};
        for(int j = 0; message[j]!= '\0' ; i++,j++){
            buffer[i] = message[j];
        }
        buffer[i] = '\0';
        return;
    }


    return;
}
