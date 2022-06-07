#include <shell.h>

typedef void (*functionPointer)(void);

static char readBuffer[BUFFER_LENGTH]={0};
typedef struct{
    char * name;
    functionPointer func;
}FunctionType;
static char param1[20]= {0};
static char param2[20] = {0};
static FunctionType programs[] = {{"fibonacci", &fibonacci},{"help",&help},{"primos", &primos},
                                  {"inforeg",&inforeg},{"div0", &div0},{"time",&time}, {"printmem", &printMem}, {0,0}};


int run(char * buffer){

    int added = addFunctions(buffer);
    if (*buffer!=0 && (added == -1 || added == -2)){
        print(buffer,RED,BLACK, 0);
        print(" is not a valid command \n", WHITE, BLACK,0);
    }
    return 1;
}

void initShell(){
    sys_scheduler("shell", &shell,"shell");
}

void shell(){
    while(1){
        print("~$ ",GREEN, BLACK,0);
		scanf(KEYBOARD_FD, readBuffer, BUFFER_LENGTH);
		run(readBuffer);
        for(int i = 0 ; i < 50000000 ; i++){}
        clearBuffer();
    }
}

void clearBuffer(){
    int i=0;
    while (readBuffer[i]!=0 && i<BUFFER_LENGTH){
        readBuffer[i]=0;
        i++;
    }


}

int addFunctions(char * buffer){
    char func1[BUFFER_LENGTH]= {0};
    char func2[BUFFER_LENGTH] = {0};
    int i1 = 0;
    int i2 = 0;
    int i3 = 0;
    int printmem;
    // char param1[10]= {0};
    // char param2[10] = {0};
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
        checkPrintMem(func1,param1);
        checkPrintMem(func2, param2);
        int func1Index = getFuncIndex(func1);
        int func2Index = getFuncIndex(func2);
        // if(printmem==1){
        //     func1Index=6;
        // }
        // if(printmem==2){
        //     func2Index=6;
        // }
        // if(printmem==3){
        //     func2Index=6;
        //     func1Index=6;
        // }
        if( func1Index != -1 && func2Index != -1){
            sys_scheduler(programs[func1Index].name, programs[func1Index].func, param1);
            sys_scheduler(programs[func2Index].name, programs[func2Index].func, param2);
            return 2;
        }
        if(func1Index == -1)
            return -1;
        //func2Index == -1
        return -2;
    }else{  //solamente se paso una funcion
        checkPrintMem(func1,param1);
        int funcIndex = getFuncIndex(func1);
        // if(printmem==1){
        //     funcIndex=6;
        // }
        if(funcIndex != -1){
            //my_printf(0,param1);
            sys_scheduler(programs[funcIndex].name, programs[funcIndex].func, param1);
            return 1;
        }
        return -1;
    }

}

void checkPrintMem(char* func1,char * parameter1){
    int flag1=1;
    char* prntmem = "printmem";
    int i;

    for(i=0;i<strlen(prntmem)&&i<strlen(func1) && flag1;i++){
        if(prntmem[i]!=func1[i]){
            flag1=0;
        }
    }
    
    if(i<strlen(prntmem) || func1[strlen(prntmem)]!=' '){
        flag1=0;
    }

    if(flag1){
        func1[strlen(prntmem)]='\0';
        int a=0;
        while(func1[a+1+strlen(prntmem)] && a<19){
            parameter1[a]=func1[a+1+strlen(prntmem)];
            a++;
        }
        parameter1[a] = '\0';
    
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
