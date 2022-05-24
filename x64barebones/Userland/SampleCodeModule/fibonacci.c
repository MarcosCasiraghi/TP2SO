#include <utilities.h>

void fibonacci(){
    int first=1;
    int second=1;
    while(1){
        for (int i = 0; i < 100000000; i++)
        {}

        int fibo= first+second;
        printDec(fibo);
        first=second;
        second=fibo;
    }
}
