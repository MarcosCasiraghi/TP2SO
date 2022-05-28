#include <utilities.h>

void fibonacci(){
    int j = 0;
    int first=1;
    int second=1;
    while(j < 10){
        for (int i = 0; i < 100000000; i++)
        {}

        int fibo= first+second;
        printDec(fibo);
        first=second;
        second=fibo;
        j++;
    }
}
