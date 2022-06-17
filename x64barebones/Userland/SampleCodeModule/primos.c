#include <utilities.h>

void primos(uint8_t fd){
    fd = 0;
    int i = 2;
    while(1)
    {

        int c=0;
        for(int j=1;j<=i;j++)
        {
            if(i%j==0)
            {
                c++;
            }
        }

       if(c==2)
        {
            my_printf("%d \n",i);
        }
        i++;
        //for (int i = 0; i < 10000000; i++)
        //{}
    }

}
