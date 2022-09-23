//
// Created by Gayba on 9/22/2022.
//

#include "include/MemoryManager.h"

typedef struct MemoryManagerCDT {
    char *nextAddress;
} MemoryManagerCDT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
    memoryManager->nextAddress = managedMemory;

    return memoryManager;
}

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
    char *allocation = memoryManager->nextAddress;

    memoryManager->nextAddress += memoryToAllocate;

    return (void *) allocation;
}
