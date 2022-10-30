#include <mmTest.h>

#define MAX_BLOCKS 128

#define MAX_MEMORY 32*1024*1024 //Mitad de la memoria total disponible

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

uint64_t mmTest(){

    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;

    printf("Inicializando Test de Memoria\n");

    while(1){
        rq = 0;
        total = 0;

        while( rq < MAX_BLOCKS && total < MAX_MEMORY){
            mm_rqs[rq].size = getUniform(MAX_MEMORY - total - 1) +1;
            mm_rqs[rq].address = sys_malloc((uint64_t) mm_rqs[rq].size);

            if (mm_rqs[rq].address == NULL) {
                printf("No memory left\n");
                return;
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
                    printf("ERROR!\n");
                }
            }
        }

        for (i = 0; i < rq; i++){
            if (mm_rqs[i].address != NULL) {
                free(mm_rqs[i].address);
            }
        }
    }

}