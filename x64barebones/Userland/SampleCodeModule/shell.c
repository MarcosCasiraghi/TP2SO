#include <shell.h>

typedef void (*functionPointer)(void);

static char readBuffer[BUFFER_LENGTH]={0};

typedef struct{
    char * name;
    functionPointer func;
    int priority;
}FunctionType;

static char param1[20]= {0};

static FunctionType programs[] = {{"fibonacci", &fibonacci, MEDIUM},
                                  {"help",&help, MEDIUM},
                                  {"primos", &primos, MEDIUM},
                                  {"invalidopcode",&invalidOpCode, HIGHEST},
                                  {"inforeg",&inforeg, MEDIUM},
                                  {"div0", &div0, HIGHEST},
                                  {"time",&time, MEDIUM},
                                  {"printmem", &printMem, MEDIUM},
                                  {"ps", &ps, MEDIUM},
                                  {"clear", &clearConsole, MEDIUM},
                                  {"mmtest", &mmTest, MEDIUM},
                                  {"mmtest2", &mmTest2, MEDIUM},
                                  {"kill", &kill, MEDIUM},
                                  {"processtest", &processTest, MEDIUM},
                                  {0,0,0}};

void run(char * buffer){
    int added = addFunctions(buffer);
    if (*buffer!=0 && (added == -1 || added == -2)){
        print(buffer,RED,BLACK);
        print(" is not a valid command \n", WHITE, BLACK);
        print("Use 'help' to know the available commands\n", WHITE,BLACK);
    }
}

void initShell(){
    sys_scheduler("shell", &shell,FOREGROUND, LOWEST,(uint64_t)"shell");
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

    int flag = 0;

        checkPrintMem(func1,param1);
        checkKill(func1, param1);
        int funcIndex = getFuncIndex(func1, &flag);
        if(funcIndex != -1){
            if (flag == 1)
                sys_scheduler(programs[funcIndex].name, programs[funcIndex].func,BACKGROUND,LOWEST, (uint64_t) param1);
            else if (flag == 0)
                sys_scheduler(programs[funcIndex].name, programs[funcIndex].func,FOREGROUND,programs[funcIndex].priority, (uint64_t) param1);
            return 1;
        }
        return -1;


}

void checkKill(char * func, char * parameter){
    char * skill = "kill";
    int i = 0;
    int flag = 1;

    for( ; i < my_strlen(skill) && i < my_strlen(func) ; i++){
        if(skill[i] != func[i]){
            flag = 0;
        }
    }

    if(i<my_strlen(skill) || func[my_strlen(skill)]!=' '){
        flag=0;
        parameter[0] = '\0';
    }

    if(flag){
        func[my_strlen(skill)]='\0';
        int a=0;
        while(func[a+1+my_strlen(skill)] && a<3){
            parameter[a]=func[a+1+my_strlen(skill)];
            a++;
        }
        parameter[a] = '\0';

        for( int b = 0 ; b < a ; b++){
            if( parameter[b] < '0' || parameter[b] > '9'){
                parameter[0] = '\0';
                return;
            }
        }

    }
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

int getFuncIndex(char * func, int * flag){
    if (func[my_strlen(func) - 1] == '&'){
        func[my_strlen(func)-1] = '\0';
        *flag = 1;
    }
    for( int i = 0 ; programs[i].name ; i++){
        if( my_strcmp(func, programs[i].name) == 0){
            return i;
        }
    }
    return -1;
}
