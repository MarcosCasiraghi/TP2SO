#include <shell.h>

typedef void (*functionPointer)(void);

typedef struct{
    char * name;
    functionPointer func;
}FunctionType;

static FunctionType programs[] = {{"fibonacci", &fibonacci},{"help",&help},{"primos", &primos},
                                  {"printreg",&printreg},{"div0", &div0},{"time",&time}, {0,0}};


int run(char * buffer){

    int added = addFunctions(buffer);
    if (*buffer!=0 && (added == -1 || added == -2)){
        print(buffer,RED,BLACK, 0);
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

int addFunctions(char * buffer){
    char func1[BUFFER_LENGTH];
    char func2[BUFFER_LENGTH];
    int i1 = 0;
    int i2 = 0;
    int i3 = 0;
    while(buffer[i3] && buffer[i3]!='|'){  //fibonacci | primos
        func1[i1++] = buffer[i3];
        i3++;
    }

    if(buffer[i3] == '|'){
        func1[--i1] = '\0';
        i3++;
        if(buffer[i3] && buffer[i3] == ' '){
            i3++;
            while(buffer[i3] ){
                func2[i2++] = buffer[i3];
                i3++;
            }
        }
        func2[i2] = '\0';
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
