//
// Created by Gayba on 9/22/2022.
//

#include "include/MemoryManager.h"

typedef struct MemoryManagerCDT {
    char *nextAddress;
} MemoryManagerCDT;

typedef struct memoryAllocations {
    pid_t pId;
    void *const restrict memStart;
    int memQty;
} memoryAllocations;

typedef struct Node{
    memoryAllocations current;
    struct Node * next;
}Node;

static Node * first;

static size_t usedMemory = 0;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
    memoryManager->nextAddress = managedMemory;

    first = malloc(sizeof(Node));

    return memoryManager;
}

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
    char *allocation = memoryManager->nextAddress;

    memoryManager->nextAddress += memoryToAllocate;

    //chequeo de que next no sea null

//    if (next.memStart - (current.memStart + current.memQty) >= memoryToAllocate) {
//        usedMemory += memoryToAllocate;
//        return (void *) allocation;
//    }
//
//    int found = 0;
//    int notAvail = 0;
//
//    While(found == 0 && notAvail == 0){
//        //recursivo pa encontrar lugar pal proceso
//        //encuentra -> found = 1
//        //no entra -> notAvail = 1
//    }
//
//
//    if (found == 1){
//        //creacion de lista y suma de usedMemory
//    }
//    else if (notAvail == 1){
//        //nada
//    }


}
