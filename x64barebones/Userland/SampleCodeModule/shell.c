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

void initShell(){
    sys_scheduler("shell", &shell);
}

void shell(){
    while(1){
        print("~$ ",GREEN, BLACK);
		char readBuffer[BUFFER_LENGTH]={0};
		scanf(KEYBOARD_FD, readBuffer, BUFFER_LENGTH);
		run(readBuffer);
    }
}
