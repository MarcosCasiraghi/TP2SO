// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <utilities.h>

void fibonacci(){
    int j = 0;
    int first=1;
    int second=1;
    my_printf("1\n1\n");
    while(1){
        for (int i = 0; i < 59000000; i++)
        {}

        int fibo= first+second;
        my_printf("%d\n",fibo);
        first=second;
        second=fibo;
        j++;
    }
}
