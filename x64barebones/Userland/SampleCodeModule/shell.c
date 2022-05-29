#include <shell.h>

typedef void (*functionPointer)(void);

typedef struct{
    char * name;
    functionPointer func;
}FunctionType;

static FunctionType programs[] = {{"fibonacci", &fibonacci},{"help",&help},{"primos", &primos}, {0,0}};


int run(char * buffer){
    for( int i = 0 ; programs[i].name ; i++){
        if( strcmp(buffer, programs[i].name) == 0){
            //programs[i].func();
            sys_scheduler(programs[i].name, programs[i].func);
            return 0;
        }
    }
    if (*buffer!=0)
    {
        print(buffer, RED, BLACK);
        print(" is not a valid command \n", WHITE, BLACK);
    }
    return 1;
}


int strcmp(char * str1, char * str2){
    int i = 0 ;
    for( ; str1[i] && str2[i]; i++){
         if(str1[i] != str2[i])
            return 1;
    }
    if( str1[i] || str2[i] )
        return 2;
    return 0;
}

