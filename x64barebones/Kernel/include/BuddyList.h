#ifndef _BUDDY_LIST_H
#define _BUDDY_LIST_H

#include <stdint.h>
#include <stdlib.h>

typedef struct BList {
  uint8_t bucket;
  uint8_t free;
  struct BList *prev, *next;
} BList;

void listInitialize(BList *list);
void listPush(BList *list, BList *entry);
void listRemove(BList *entry);
BList * listPop(BList *list);
char listIsEmpty(BList *list);

#endif