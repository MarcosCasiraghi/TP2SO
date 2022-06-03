#include <shell.h>

typedef void (*functionPointer)(void);

typedef struct{
    char * name;
    functionPointer func;
}FunctionType;

static FunctionType programs[] = {{"fibonacci", &fibonacci},{"help",&help},{"primos", &primos},{"printreg",&printreg}, {0,0}};


int run(char * buffer){
    char func1[BUFFER_LENGTH];
    char func2[BUFFER_LENGTH];

    int added = addFunctions(func1, func2, buffer);
    if (*buffer!=0 && (added == -1 || added == -2)){
        if(added == -1){
            print(func1, RED, BLACK,0);
        }else{
            print(func2, RED, BLACK,0);
        }
        print(" is not a valid command \n", WHITE, BLACK,0);
    }
    return 1;
}

void initShell(){
    sys_scheduler("shell", &shell,0);
}

void shell(){
    while(1){
        print("~$ ",GREEN, BLACK,0);
		char readBuffer[BUFFER_LENGTH]={0};
		scanf(KEYBOARD_FD, readBuffer, BUFFER_LENGTH);
		run(readBuffer);
        for(int i = 0 ; i < 50000000 ; i++){}
    }
}

int addFunctions(char * func1, char * func2, char * buffer){
    int i1 = 0;
    int i2 = 0;
    while(buffer[i1] && buffer[i1]!='|'){  //fibonacci | primos
        if(buffer[i1] != ' ')
            func1[i1] = buffer[i1];
        i1++;
    }
    func1[i1] = '\0';
    if(buffer[i1] == '|'){
        i1++;
        while(buffer[i1] ){
            if(buffer[i1] != ' ')
                func2[i2++] = buffer[i1];
            i1++;
        }
        //No hace falta agregarle 0 al final, buffer se lo agrega
        int func1Index = getFuncIndex(func1);
        int func2Index = getFuncIndex(func2);
        if( func1Index != -1 && func2Index != -1){
            sys_scheduler(programs[func1Index].name, programs[func1Index].func, 1);
            sys_scheduler(programs[func2Index].name, programs[func2Index].func, 2);
            return 2;
        }
        if(func1Index == -1)
            return -1;
        //func2Index == -1
        return -2;
    }else{  //solamente se paso una funcion
        int funcIndex = getFuncIndex(func1);
        if(funcIndex != -1){
            sys_scheduler(programs[funcIndex].name, programs[funcIndex].func, 0);
            return 1;
        }
        return -1;
    }

}

int getFuncIndex(char * func){
    for( int i = 0 ; programs[i].name ; i++){
        if( strcmp(func, programs[i].name) == 0){
            return i;
        }
    }
    return -1;
}
