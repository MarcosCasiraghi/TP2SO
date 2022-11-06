#include <semaphores.h>


semaphore * semList;

static semaphore * getSem(int id);
static semaphore * newSem(int id, int value);
static void addSem(semaphore * sem);
static void realeseSem(semaphore *sem);
static void removeSem(semaphore * sem);


int wait(int id){
    semaphore * aux = getSem(id);
    if(aux==NULL){
        return -1;
    }
    mutex_lock(&(aux->mutex));
    if(aux->value > 0){
        aux->value--;
        mutex_unlock(&(aux->mutex));
    }
    else{
        int pid = getPID();
        aux->blockedProccesses[aux->blockCounter++] = pid;
        mutex_unlock(&(aux->mutex));
        blockProcess(pid);
    }
    return 0;
}

void closeAllSem(){
    semaphore * first = semList;
    while (first != NULL){
        removeSem(first);
        first = first->tail;
    }
    semList = NULL;
    return;
}

int waitBack(int id){
    semaphore * aux = getSem(id);
    if(aux==NULL){
        return -1;
    }
    mutex_lock(&(aux->mutex));
    if(aux->value > 0){
        aux->value--;
        mutex_unlock(&(aux->mutex));
    }
    else{
        int pid = getPID();
        aux->blockedProccesses[aux->blockCounter++] = pid;
        mutex_unlock(&(aux->mutex));
        blockProcessTick(pid);
    }
    return 0;
}

int post(int id){
    semaphore * aux = getSem(id);
    if(aux== NULL){
        return -1;
    }
    mutex_lock(&(aux->mutex));
    if(aux->blockCounter>0){
        realeseSem(aux);
    }
    else{
        aux->value++;
    }
     mutex_unlock(&(aux->mutex));
    return 0;

}

int semClose(int id){
    semaphore * aux = getSem(id);
    if(aux==NULL){
        return -1;
    }
    if(aux->openCounter>1){
        aux->openCounter--;
    }
    else{
        removeSem(aux);
    }
    return 0;
}

int semOpen(int  id, int value){
    semaphore * aux = getSem(id);
    if(aux == NULL){
        aux = newSem(id, value);
        if(aux==NULL){
            return-1;
        }
    }
    aux->openCounter++;
    return id;

}

void semStatus(char * buffer){
    char title[] = {"Active Semaphores:\n\n"};
    int i;
    for(i = 0; title[i]!= '\0' ; i++){
        buffer[i] = title[i];
    }

    semaphore * sem = semList;
    if(sem == NULL){
        char message[] = {"No active semaphores\n"};
        for(int j = 0; message[j]!= '\0' ; i++,j++){
            buffer[i] = message[j];
        }
        return;
    }

    while(sem){
        char idString[] = {"ID: "};
        for(int j = 0; idString[j]!= '\0' ; i++,j++){
            buffer[i] = idString[j];
        }

        char id[5] = {0};
        uintToBase(sem->id, id, 10);
        for(int j = 0; id[j]!= '\0' ; i++,j++){
            buffer[i] = id[j];
        }

        char valueString[] = {"\nValue: "};
        for(int j = 0; valueString[j]!= '\0' ; i++,j++){
            buffer[i] = valueString[j];
        }

        char value[5] = {0};
        uintToBase(sem->value, value, 10);
        for(int j = 0; value[j]!= '\0' ; i++,j++){
            buffer[i] = value[j];
        }

        char amountString[] = {"\nAmount of Processes Involved: "};
        for(int j = 0; amountString[j]!= '\0' ; i++,j++){
            buffer[i] = amountString[j];
        }

        char amount[5] = {0};
        uintToBase(sem->openCounter, amount, 10);
        for(int j = 0; amount[j]!= '\0' ; i++,j++){
            buffer[i] = amount[j];
        }

        char blockedString[] = {"\nAmount of Processes Blocked: "};
        for(int j = 0; blockedString[j]!= '\0' ; i++,j++){
            buffer[i] = blockedString[j];
        }

        char blocked[5] = {0};
        uintToBase(sem->blockCounter, blocked, 10);
        for(int j = 0; blocked[j]!= '\0' ; i++,j++){
            buffer[i] = blocked[j];
        }

        if(sem->blockCounter > 0){
            char blockedS[] = {"\nBlocked Processes:\n"};
            for(int j = 0; blockedS[j]!= '\0' ; i++,j++){
                buffer[i] = blockedS[j];
            }

            for( int k = 0 ; k < sem->blockCounter ; k++){
                char pidString[] = {"    PID: "};
                for(int j = 0; pidString[j]!= '\0' ; i++,j++){
                    buffer[i] = pidString[j];
                }

                char pid[5] = {0};
                uintToBase(sem->blockedProccesses[k], pid, 10);
                for(int j = 0; pid[j]!= '\0' ; i++,j++){
                    buffer[i] = pid[j];
                }

                char end[] = {"\n"};
                for(int j = 0; end[j]!= '\0' ; i++,j++){
                    buffer[i] = end[j];
                }
            }
        }
        char newLine[] = {"\n"};
        buffer[i++]=newLine[0];
        
        
        sem = sem->tail;
    }
    return;
};//TODO

static semaphore * getSem(int id){
    semaphore * aux = semList;
    while(aux){
        if(aux->id == id){
            return aux;
        }
        aux = aux->tail;
    }
    return NULL;
}

static void addSem(semaphore * sem){
    semaphore *aux = semList;
    if(aux==NULL){
        semList=sem;
        return;
    }
    while(aux->tail!=NULL){
        aux=aux->tail;
    }
    aux->tail = sem;
}

static semaphore * newSem(int id, int value){
    semaphore * aux = allocMemory(sizeof(semaphore));
    if(aux==NULL){
        return NULL;
    }
    aux->id=id;
    aux->value=value;
    aux->blockCounter = 0;
    aux->openCounter = 0 ;
    aux->mutex = 0;
    aux->tail=NULL;
    addSem(aux);
    return aux;
}

static void removeSem(semaphore * sem){
    semaphore * aux = semList;
    if(aux == sem){
        semList=aux->tail;
        freeMemory(sem);
        return;
    }
    while(aux->tail != sem){
        aux=aux->tail;
    }
    aux->tail = sem->tail;
    freeMemory(sem);


}

static void realeseSem(semaphore *sem){
    int pid =sem->blockedProccesses[0];
    for(int i=0;i<sem->blockCounter -1;i++){
        sem->blockedProccesses[i]=sem->blockedProccesses[i+1];
    }
    sem->blockCounter--;
    unblockProcess(pid);
}