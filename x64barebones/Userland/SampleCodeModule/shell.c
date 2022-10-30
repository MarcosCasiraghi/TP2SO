#include <shell.h>

typedef void (*functionPointer)(void);

static char readBuffer[BUFFER_LENGTH]={0};

typedef struct{
    char * name;
    functionPointer func;
    int ground;
    int priority;
}FunctionType;

static char param1[20]= {0};

static FunctionType programs[] = {{"fibonacci", &fibonacci, FOREGROUND, MEDIUM},
                                  {"help",&help, FOREGROUND, MEDIUM},
                                  {"primos", &primos, FOREGROUND, MEDIUM}, 
                                  {"invalidopcode",&invalidOpCode, FOREGROUND, HIGHEST},
                                  {"inforeg",&inforeg, FOREGROUND, MEDIUM},
                                  {"div0", &div0, FOREGROUND, HIGHEST},
                                  {"time",&time, FOREGROUND, MEDIUM}, 
                                  {"printmem", &printMem, FOREGROUND, MEDIUM},
                                  {"mmtest", &mmTest, FOREGROUND, MEDIUM},
                                  {0,0,0,0}};

void run(char * buffer){
    int added = addFunctions(buffer);
    if (*buffer!=0 && (added == -1 || added == -2)){
        print(buffer,RED,BLACK);
        print(" is not a valid command \n", WHITE, BLACK);
        print("Use 'help' to know the available commands\n", WHITE, BLACK);
    }
}

void initShell(){
    sys_scheduler("shell", &shell,FOREGROUND,LOWEST,(uint64_t)"shell");
}

void shell(){
    while(1){
        print("~$ ",GREEN, BLACK);
		my_scanf(readBuffer, BUFFER_LENGTH);
		run(readBuffer);
        sys_exec();
        //for(int i = 0 ; i < 0 ; i++){}
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

    while (buffer[i3]){
        func1[i1++] = buffer[i3];
        i3++;
    }

        checkPrintMem(func1,param1);
        int funcIndex = getFuncIndex(func1);  //TODO
        if(funcIndex != -1){
            sys_scheduler(programs[funcIndex].name, programs[funcIndex].func,programs[funcIndex].ground,programs[funcIndex].priority, (uint64_t) param1); //TODO
            return 1;
        }
        return -1;


}

void checkPrintMem(char* func1,char * parameter1){
    int flag1=1;
    char* prntmem = "printmem";
    int i;

    for(i=0;i<my_strlen(prntmem)&&i<my_strlen(func1) && flag1;i++){
        if(prntmem[i]!=func1[i]){
            flag1=0;
        }
    }

    if(i<my_strlen(prntmem) || func1[my_strlen(prntmem)]!=' '){
        flag1=0;
        parameter1[0] = '\0';
    }

    if(flag1){
        func1[my_strlen(prntmem)]='\0';
        int a=0;
        while(func1[a+1+my_strlen(prntmem)] && a<19){
            parameter1[a]=func1[a+1+my_strlen(prntmem)];
            a++;
        }
        parameter1[a] = '\0';

    }

}

int getFuncIndex(char * func){
    for( int i = 0 ; programs[i].name ; i++){
        if( my_strcmp(func, programs[i].name) == 0){
            return i;
        }
    }
    return -1;
}
