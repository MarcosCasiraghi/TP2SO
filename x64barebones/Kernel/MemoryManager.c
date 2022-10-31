#include <MemoryManager.h>

static size_t heapsize;
static char * memoryManagerModuleAdress = NULL;


typedef long Aling; //para alinear al limite mayor

typedef union header {      //ecabezado del bloque
    struct {
        union header *ptr;  //puntero al siguiente bloque si esta en la lista libre
        unsigned size;
    }s;
    Aling x;    //obliga la alineacion de bloques
} Header;


static Header * base;         //lista vacia para iniciar
static Header * freeList = NULL;    //inicio de una lista libre

size_t tUnits;  

void initializeMemoryManager(char * startAddress, size_t size){
    heapsize=size;
    memoryManagerModuleAdress=startAddress;
    if(startAddress == NULL){
        return;
    }
    tUnits = (size + sizeof(Header) -1) / sizeof(Header) +1;
    freeList = base = (Header *)startAddress;
    freeList->s.size = tUnits;
    freeList->s.ptr = freeList;
}
void freeAll(){
    if(memoryManagerModuleAdress==NULL){
        return;
    }
    initializeMemoryManager(memoryManagerModuleAdress,heapsize);

}

void allocMemory(uint64_t nbytes, void ** addressToRet){
    if(nbytes == 0){
        *addressToRet = NULL;
        return;
    }

    Header *Cnode, *prevPtr;
    size_t nunits;
    void * result;
    bool is_allocating;

    nunits = (nbytes + sizeof(Header)-1 )/sizeof(Header) +1;

    prevPtr = freeList;

    is_allocating = true;
    for( Cnode = prevPtr->s.ptr ; is_allocating ; Cnode = Cnode->s.ptr){
        if( Cnode->s.size >= nunits){
            if( Cnode->s.size == nunits){
                prevPtr->s.ptr = Cnode->s.ptr;
            }else {
                Cnode->s.size -= nunits;
                Cnode += Cnode->s.size;
                Cnode->s.size = nunits;
            }

            freeList = prevPtr;
            result = Cnode + 1;
            is_allocating = false;
        }
        if( Cnode == freeList){
            addressToRet = NULL;
            return;
        }

        prevPtr = Cnode;
    }

    *addressToRet = result;
    // return result;
}

void freeMemory( void * block){
    //se fija que la dirreccion sea valida al ser el principio de un bloque
    if( block == NULL || (((long) block - (long)base) % sizeof(Header)) != 0){
        return;
    }

    Header *Cnode, *freeBlock;
    freeBlock = (Header *)block -1;

    if( freeBlock < base || freeBlock >= (base + tUnits * sizeof(Header)) ){
        return;
    }

    block = NULL;

    bool external = false;

    for( Cnode = freeList ; !(freeBlock > Cnode && freeBlock < Cnode->s.ptr); Cnode = Cnode->s.ptr){
        if( freeBlock == Cnode || freeBlock == Cnode->s.ptr){
            return;
        }

        if( Cnode >= Cnode->s.ptr && (freeBlock > Cnode || freeBlock < Cnode->s.ptr)){
            external = true;
            break;
        }
    }

    if(!external && (Cnode + Cnode->s.size > freeBlock || 
        freeBlock + freeBlock->s.size > Cnode->s.ptr)){
        return;
    }

    if( freeBlock + freeBlock->s.size == Cnode->s.ptr){
        freeBlock->s.size += Cnode->s.ptr->s.size;
        freeBlock->s.ptr = Cnode->s.ptr->s.ptr;
    }else {
        freeBlock->s.ptr = Cnode->s.ptr;
    }

    if( Cnode + Cnode->s.size == freeBlock){
        Cnode->s.size += freeBlock->s.size;

        Cnode->s.ptr = freeBlock->s.ptr;
    }else{
        Cnode->s.ptr = freeBlock;
    }

    freeList = Cnode;
}

void memoryDump(char * buffer){
    int blockNumber = 1;
    Header * OG, *current;
    OG = current = freeList;
    int flag = 1;

    int i = 0, j = 0;
    char title[] = {"Memory Dump (Free List Memory Manager)\n Total Memory: "};

    for(;title[j] != '\0'; i++, j++){
        buffer[i] = title[j];
    }

    char size[15] = {0};
    uintToBase((uint32_t)tUnits * sizeof(Header), size, 10);
    j = 0;
    for(; size[j]!= 0 ; i++, j++){
        buffer[i] = size[j];
    }

    char end[] = {" bytes \n\n"};
    j = 0;
    for( ; end[j] != '\0'; i++, j++){
        buffer[i] = end[j];
    }

    if( freeList == NULL){
        char noBlocksMessage[] = {"No Free Blocks available\n"};
        j = 0;
        for( ; noBlocksMessage[j] != '\0'; i++, j++){
            buffer[i] = noBlocksMessage[j];
        }
        return;
    }

    char blocks[] = {"Free Blocks: \n"};
    j = 0;
    for( ; blocks[j] != '\0'; i++, j++){
        buffer[i] = blocks[j];
    }

    while( current != OG || flag){
        flag = 0;
        char text[] = {"Block Number: "};
        j = 0;
        for( ; text[j]!= '\0'; i++, j++){
            buffer[i] = text[j];
        }

        char number[5] = {0};
        uintToBase(blockNumber, number, 10);
        j = 0;
        for(; number[j]!= 0 ; i++, j++){
            buffer[i] = number[j];
        }

        char text2[] = {"\nBase: "};
        j = 0;
        for( ; text2[j]!= '\0'; i++, j++){
            buffer[i] = text2[j];
        }

        char address[10] = {0};
        uintToBase(current, address, 16);
        j = 0;
        for(; address[j]!= 0 ; i++, j++){
            buffer[i] = address[j];
        }

        char text3[] = {"\nFree Units: "};
        j = 0;
        for( ; text3[j]!= '\0'; i++, j++){
            buffer[i] = text3[j];
        }

        char size[10] = {0};
        uintToBase(current->s.size, size, 10);
        j = 0;
        for(; address[j]!= 0 ; i++, j++){
            buffer[i] = address[j];
        }

        char text4[] = {"\n\n"};
        j = 0;
        for( ; text4[j]!= '\0'; i++, j++){
            buffer[i] = text[j];
        }

        current = current->s.ptr;
        blockNumber++;
    }

}
