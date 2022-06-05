#include <utilities.h>

void fibonacci(uint8_t fd){
    fd = 0;
    int j = 0;
    int first=1;
    int second=1;
    my_printf(fd,"1\n1\n");
    while(1){
        for (int i = 0; i < 59000000; i++)
        {}

        int fibo= first+second;
        my_printf(fd,"%d\n",fibo);
        first=second;
        second=fibo;
        j++;
    }
}
