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
    if(aux->openCounter>0){
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

void semStatus(){
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
    blockProcess(pid);
}