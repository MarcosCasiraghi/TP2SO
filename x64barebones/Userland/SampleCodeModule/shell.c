#include <shell.h>

typedef void (*functionPointer)(void);

static char readBuffer[BUFFER_LENGTH]={0};

static int initialPipeId = 1000;

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
                                  {"semtest1", &semTest1, MEDIUM},
                                  {"semtest2", &semTest2, MEDIUM},
                                  {"prioritytest", &priorityTest, MEDIUM},
                                  {"semtest",&test_sync,MEDIUM},
                                  {"semstatus", &semStatus, MEDIUM},
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
    char * params[] = {"shell", NULL};
    sys_scheduler(&shell,FOREGROUND, SHELLPRIO, 1, params);
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

    char * argv[MAX_PARAMS] = {0};
    char * argv2[MAX_PARAMS] = {0};
    int background = 0;
    int background2 = 0;

    while (buffer[i3] && buffer[i3] != '|'){
        func1[i1++] = buffer[i3];
        i3++;
    }

    if(buffer[i3] == '|') {
        func1[--i1] = '\0';
        i3++;
        if (buffer[i3] && buffer[i3] == ' ') {
            i3++;
            while (buffer[i3]) {
                func2[i2++] = buffer[i3];
                i3++;
            }
        }
        func2[i2] = '\0';

        int func1Index = getFuncIndex(func1, &background);
        int func2Index = getFuncIndex(func2, &background2);

        if (func1Index != -1 && func2Index != -1){  //TODO: chequear que sea de las funciones disponibles con el pipe
            int pipeId = sys_pipeOpen(initialPipeId);
            uint64_t argc = createArgv(programs[func1Index].name, pipeId, argv);
            uint64_t argc2 = createArgv(programs[func2Index].name, pipeId, argv2);

            if (background == 0)
                sys_scheduler(programs[func1Index].func,FOREGROUND,programs[func1Index].priority, argc, argv );
            else if (background == 1 )
                sys_scheduler(programs[func1Index].func,BACKGROUND,programs[func1Index].priority, argc, argv );

            if (background2 == 0){
                sys_scheduler(programs[func2Index].func,FOREGROUND,programs[func2Index].priority, argc2, argv2 );
            }
            if (background2 == 0){
                sys_scheduler(programs[func2Index].func,FOREGROUND,programs[func2Index].priority, argc2, argv2 );
            }
            initialPipeId++;

            return 2;
        }
        return -1;
    }


    int specialFunc = 0;

    checkSpecialFunc("kill",func1, param1, &specialFunc);
    if( specialFunc == 0){
        checkSpecialFunc("printmem",func1,param1, &specialFunc);
    }
    if( specialFunc == 0){
        checkSpecialFunc("semtest",func1,param1, &specialFunc);
    }
    int funcIndex = getFuncIndex(func1, &background);
    if(funcIndex != -1){
        if (background == 1){
            uint64_t argc = createArgv(programs[funcIndex].name, param1, argv);
            sys_scheduler(programs[funcIndex].func,BACKGROUND,programs[funcIndex].priority, argc, argv );
        
        }
        else if (background == 0){
            uint64_t argc;
            argc = createArgv(programs[funcIndex].name, param1, argv);
            sys_scheduler( programs[funcIndex].func,FOREGROUND,programs[funcIndex].priority, argc, argv);
        }
        return 1;
    }
    return -1;


}

int createArgv(char * name, char * params, char ** argv){
    int argc = 1;

    argv[0] = name;

    int i = 0;
    int j = 1;
    int k = 0;
    while(params[i] != '\0'){
        while( params[i] != ' ' && params[i] != '\0'){
            i++;
        }
        if(params[i] == ' '){
            params[i++] = '\0';
            argv[j++] = params+k;
            k = i;
            argc++;
        }else{
            argv[j++] = params+k;
            argc++;
            return argc;
        }
    }
    return argc;
}

void checkSpecialFunc(char * name, char * input, char * parameter, int * flag){
    int i = 0;
    int flag1 = 1;

    for( ; i < my_strlen(name) && i < my_strlen(input) ; i++){
        if(name[i] != input[i]){
            flag1 = 0;
        }
    }

    if(i<my_strlen(name) || input[my_strlen(name)]!=' '){
        flag1=0;
        parameter[0] = '\0';
    }

    if(flag1){
        *flag = 1;
        input[my_strlen(name)]='\0';
        int a=0;
        while(input[a+1+my_strlen(name)] && a<19){
            parameter[a]=input[a+1+my_strlen(name)];
            a++;
        }
        parameter[a] = '\0';

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
