#include <primoFibo.h>

#define LEFT 0
#define RIGHT 1

static int fiboFirst = 0;
static int fiboSecond = 1;
static int currentPrimo = 2;

void primoFibo(){
    int orden = 1;
    while (1) {
        nextFibo(orden);
        nextPrimo(orden);
    }
}

void nextFibo(int orden){
    my_printf(orden,"%d\n",fiboFirst+fiboSecond);
    for (int i = 0; i < 10000000; i++)
    {}
}

void nextPrimo(int orden){
    int flag = 1;
    while(flag) {
        int c = 0;
        for (int k = 1; k <= currentPrimo; k++) {
            if (currentPrimo % k == 0) {
                c++;
            }
        }

        if (c == 2) {
            my_printf(orden+1,"%d\n",currentPrimo);
            flag=0;
            for (int i = 0; i < 10000000; i++)
            {}
        }
        currentPrimo++;
    }

}