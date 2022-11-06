// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef BUDDY_MANAGER

#include <BuddyList.h>
#include <MemoryManager.h>

#define MIN_ALLOC_LOG2 4
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

#define MAX_ALLOC_LOG2 31 //2GB?

#define MAX_BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2)

static size_t heapSize;
static char * memoryManagerModuleAddress = NULL;

// tree levels
static BList buckets[MAX_BUCKET_COUNT];
static size_t maximum_bucket_size;
static BList *base_ptr;
static uint8_t buckets_amount;

static void addNodeToBucket(BList *bucketList, BList *node, uint8_t bucketLevel);
static size_t getMinimumSuitableBucket(size_t request);
static int getAvailableBucket(uint8_t minBucketRequired);
static BList *getNodeBuddy(BList *node);
static BList *getNodeAddress(BList *node);  
static int log2(uint32_t number);

void initializeMemoryManager( char * startAddress, size_t size){
    if (startAddress == NULL) {
    return;
  }

  memoryManagerModuleAddress = startAddress;
  heapSize = size;

  base_ptr = (BList *)startAddress;
  maximum_bucket_size = size;

  buckets_amount = (int)log2(size) - MIN_ALLOC_LOG2 + 1;

  if (buckets_amount > MAX_BUCKET_COUNT) {
    buckets_amount = MAX_BUCKET_COUNT;
  }

  for (int i = 0; i < buckets_amount; i++) {
    listInitialize(&buckets[i]);
    buckets[i].free = 0;
    buckets[i].bucket = i;
  }

  addNodeToBucket(&buckets[buckets_amount - 1], base_ptr, buckets_amount - 1);
}

void * allocMemory( const uint64_t nbytes){
    size_t bytesNeeded = nbytes + sizeof(BList);

    if (nbytes == 0 || bytesNeeded > maximum_bucket_size + 1) {
        return NULL;
    }

    uint8_t idealBucket = getMinimumSuitableBucket(bytesNeeded);
    int availableBucket = getAvailableBucket(idealBucket);

    if (availableBucket == -1) {
        return NULL;
    }

    BList *resultNode = listPop(&buckets[availableBucket]);

    for (; idealBucket < availableBucket; availableBucket--) {
        resultNode->bucket--;
        addNodeToBucket(&buckets[availableBucket - 1], getNodeBuddy(resultNode), availableBucket - 1);
    }

    resultNode->free = 0;

    return (void *)(resultNode + 1);
}

void freeMemory(void *block) {
  if (block == NULL) {
    return;
  }

  BList *freeNode = (BList *)block - 1;

  freeNode->free = 1;

  BList *freeNodeBuddy = getNodeBuddy(freeNode);

  while (freeNode->bucket != buckets_amount - 1 && freeNodeBuddy->bucket == freeNode->bucket && freeNodeBuddy->free) {
    listRemove(freeNodeBuddy);
    freeNode = getNodeAddress(freeNode);
    freeNode->bucket++;
    freeNodeBuddy = getNodeBuddy(freeNode);
  }

  listPush(&buckets[freeNode->bucket], freeNode);
}

static void addNodeToBucket(BList *bucketList, BList *node, uint8_t bucketLevel) {
  node->bucket = bucketLevel;
  node->free = 1;
  listPush(bucketList, node);
}

static size_t getMinimumSuitableBucket(size_t request) {
  size_t requestLog2 = log2(request);

  if (requestLog2 < MIN_ALLOC_LOG2) {
    return 0;
  }

  requestLog2 -= MIN_ALLOC_LOG2;

  if (request && !(request & (request - 1))) {
    return requestLog2;
  }

  return requestLog2 + 1;
}

static int getAvailableBucket(uint8_t minBucketRequired) {
  uint8_t availableBucket;

  for (availableBucket = minBucketRequired;
       availableBucket < buckets_amount &&
       listIsEmpty(&buckets[availableBucket]);
       availableBucket++)
    ;

  if (availableBucket > buckets_amount) {
    return -1;
  }

  return availableBucket;
}

static BList *getNodeBuddy(BList *node) {
  uint8_t bucket = node->bucket;
  uintptr_t nodeCurrentOffset = (uintptr_t)node - (uintptr_t)base_ptr;
  uintptr_t nodeNewOffset = nodeCurrentOffset ^ (1 << (MIN_ALLOC_LOG2 + bucket));

  return (BList *)((uintptr_t)base_ptr + nodeNewOffset);
}

static BList *getNodeAddress(BList *node) {
  uint8_t bucket = node->bucket;
  uintptr_t mask = (1 << (MIN_ALLOC_LOG2 + bucket));
  mask = ~mask;

  uintptr_t nodeCurrentOffset = (uintptr_t)node - (uintptr_t)base_ptr;
  uintptr_t nodeNewOffset = nodeCurrentOffset & mask;

  return (BList *)(nodeNewOffset + (uintptr_t)base_ptr);
}

void memoryDump(char * buffer){
  BList *list, *listAux;
  uint32_t idx = 0;
  uint32_t spaceAvailable = 0;

  int k = 0, j = 0;
  char title[] = {"Memory Dump (Buddy Memory Manager)\nBuckets with free Blocks:\n\n"};
  for( ; title[j]!= '\0'; j++, k++){
    buffer[k] = title[j];
  }

  for (int i = buckets_amount - 1; i >= 0; i--) {
    list = &buckets[i];
    if (!listIsEmpty(list)) {
      char bucket[] = {"Bucket "};
      for(j=0 ; bucket[j]!= '\0'; j++, k++){
        buffer[k] = bucket[j];
      }
      char bucketString[15] = {0};
      uintToBase(i + MIN_ALLOC_LOG2, bucketString, 10);
      for(j=0 ; bucketString[j]!= 0; j++, k++){
        buffer[k] = bucketString[j];
      }

      char freeBlocksString[] = {"\nFree blocks of size 2^"};
      for(j=0 ; freeBlocksString[j]!= '\0'; j++, k++){
        buffer[k] = freeBlocksString[j];
      }

      for(j=0 ; bucketString[j]!= 0; j++, k++){
        buffer[k] = bucketString[j];
      }

      char newLine[] = {"\n"};
      for(j=0 ; newLine[j]!= '\0'; j++, k++){
        buffer[k] = newLine[j];
      }

      // printf("    Bucket %d\n", i + MIN_ALLOC_LOG2);
      // printf("    Free blocks of size 2^%d\n", i + MIN_ALLOC_LOG2);
      for (listAux = list->next, idx = 1; listAux != list;
           idx++, listAux = listAux->next) {
        if (listAux->free) {
          char block[] = {"Block Number: "};
          for(j=0 ; block[j]!= '\0'; j++, k++){
            buffer[k] = block[j];
          }

          char idxString[15] = {0};
          uintToBase(idx, idxString, 10);
          for(j=0 ; idxString[j]!= 0; j++, k++){
            buffer[k] = idxString[j];
          }

          char state[] = {"\nState: free\n\n"};
          for(j=0 ; state[j]!= '\0'; j++, k++){
            buffer[k] = state[j];
          }
          // printf("        Block number: %d\n", idx);
          // printf("        State: free\n\n");
          spaceAvailable += idx * (1 << (MIN_ALLOC_LOG2 + i));
        }
      }
    }
  }
  char availableSpace[] = {"Available Space: "};
  for(j=0 ; availableSpace[j]!= '\0'; j++, k++){
    buffer[k] = availableSpace[j];
  }

  char spaceString[15] = {0};
  uintToBase(spaceAvailable, spaceString, 10);
  for(j=0 ; spaceString[j]!= 0; j++, k++){
    buffer[k] = spaceString[j];
  }

  char end[] = {"\n\n"};
  for(j=0 ; end[j]!= 0; j++, k++){
    buffer[k] = end[j];
  }
  // printf("\nAvailable space: %d\n\n", spaceAvailable);
}

static int log2(uint32_t number) {
  if (number == 0) {
    return -1;
  }

  int result = -1;
  while (number) {
    result++;
    number >>= 1;
  }
  return result;
}

void freeAll(){
  if(memoryManagerModuleAddress==NULL){
    return;
  }
  initializeMemoryManager(memoryManagerModuleAddress,heapSize);
}

#endif
