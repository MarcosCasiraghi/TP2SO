// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <BuddyList.h>

void listInitialize(BList *list) {
  list->prev = list;
  list->next = list;
}

void listPush(BList *list, BList *entry) {
  BList *prev = list->prev;
  entry->prev = prev;
  entry->next = list;
  prev->next = entry;
  list->prev = entry;
}

void listRemove(BList *entry) {
  BList *prev = entry->prev;
  BList *next = entry->next;
  prev->next = next;
  next->prev = prev;
}

BList *listPop(BList *list) {
  BList *back = list->prev;
  if (back == list) {
    return NULL;
  }
  listRemove(back);
  return back;
}

char listIsEmpty(BList *list) { return list->prev == list; }