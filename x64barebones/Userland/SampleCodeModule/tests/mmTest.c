#include <mmTest.h>

#define MAX_BLOCKS 128

// #define MAX_MEMORY 1024*1024*32
#define MAX_MEMORY (1048576 / 2)

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void mmTest(){

    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;

    print("Inicializando Test de Memoria\n", WHITE,BLACK);

    int counter = 0;

    while(1){
        my_printf("Cycle Number: %d\n", counter++);
        rq = 0;
        total = 0;

        while( rq < MAX_BLOCKS && total < MAX_MEMORY){
            mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) +1;
            mm_rqs[rq].address = sys_malloc((uint64_t) mm_rqs[rq].size);

            if (mm_rqs[rq].address == NULL) {
                print("No memory left\n", RED,BLACK);
                exit();
            }
            total += mm_rqs[rq].size;
            rq++;
        }

        uint32_t i;
        for (i = 0; i < rq; i++){
            if (mm_rqs[i].address != NULL) {
                memset(mm_rqs[i].address, i, mm_rqs[i].size);
            }
        }

        for (i = 0; i < rq; i++){
            if (mm_rqs[i].address != NULL) {
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
                    print("ERROR!\n", RED,WHITE);
                }
            }
        }

        for (i = 0; i < rq; i++){
            if (mm_rqs[i].address != NULL) {
                sys_free(mm_rqs[i].address);
            }
        }

    }

}

void mmTest2(){
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq = 0;
    uint32_t total = 0;

    print("Inicializando Test de Memoria\n", WHITE,BLACK);

    while( rq < MAX_BLOCKS && total < MAX_MEMORY){
            mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) +1;
            mm_rqs[rq].address = sys_malloc((uint64_t) mm_rqs[rq].size);

            if (mm_rqs[rq].address == NULL) {
                print("No memory left\n", RED,BLACK);
                exit();
            }
            total += mm_rqs[rq].size;
            rq++;
    }

    uint32_t i;
    for (i = 0; i < rq; i++){
        if (mm_rqs[i].address != NULL) {
            memset(mm_rqs[i].address, i, mm_rqs[i].size);
        }
    }

    for (i = 0; i < rq; i++){
        if (mm_rqs[i].address != NULL) {
            if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
                print("ERROR!\n", RED,WHITE);
            }
        }
    }

    char dump[1000] = {'\0'};
    sys_memStatus(dump);
    my_printf("%s\n", dump);

    for (i = 0; i < rq; i++){
        if (mm_rqs[i].address != NULL) {
            sys_free(mm_rqs[i].address);
        }
    }
    exit();
}