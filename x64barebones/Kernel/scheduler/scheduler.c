// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <scheduler.h>

#define STACK_SIZE 4096
#define REGISTERS 20
#define MAX_TASKS 16
#define READY 0
#define BLOCKED 1
#define KILLED 2
#define BUFFERSIZE 300
#define HIGHEST 0
#define MEDIUM 1
#define LOWEST 2
#define SHELLPRIO 3
#define IDLEPRIO 4
#define FOREGROUND 0
#define BACKGROUND 1

#define HIGHESTTICKS 3
#define MEDIUMTICKS 2
#define LOWESTTICKS 1

typedef void (*functionPointer)(int, char **);

typedef struct{
    functionPointer func;
    int present;
    int status;
    int pID;
    char * name;
    uint64_t argc;
    char ** argv;
    int priority;
    int ground;
}FunctionType;

static int activePID = 0;
static FunctionType tasks[MAX_TASKS];
static int priorityTickers[3] = {0};


static char stack[MAX_TASKS+1][STACK_SIZE] = {{0}};
static uint64_t reg[MAX_TASKS+1][REGISTERS] ={{0}};
static int processes=0;
static int pIDCounter = 0;

void ps(char * result){
   int counter = 0;
   for (int i = 0; i < MAX_TASKS; i++){
       if (tasks[i].present == 1 && tasks[i].status != KILLED){
           for(int j = 0; tasks[i].name[j] != '\0'; j++){
               result[counter++] = tasks[i].name[j];
           }

           result[counter++] = ' ';

           char auxBuffer[BUFFERSIZE] = {'\0'};

           uintToBase( tasks[i].pID,auxBuffer, 10);

           for(int j = 0;auxBuffer[j] != '\0'; j++){
               result[counter++] = auxBuffer[j];
           }

           result[counter++] = ' ';

           uintToBase(tasks[i].priority,auxBuffer, 10);

           for(int j = 0;auxBuffer[j] != '\0'; j++){
               result[counter++] = auxBuffer[j];
           }

           result[counter++] = ' ';

           char * fore = "Foreground";
           char * back = "Background";

           if (tasks[i].ground == FOREGROUND){
               for (int k = 0; fore[k]!='\0'; ++k) {
                   result[counter++] = fore[k];
               }
           }
           else{
               for (int k = 0; back[k]!='\0'; ++k) {
                   result[counter++] = back[k];
               }
           }

           result[counter++] = ' ';


           uintToBase( reg[i][7],auxBuffer, 16);

           for(int j = 0;auxBuffer[j] != '\0'; j++){
               result[counter++] = auxBuffer[j];
           }

           result[counter++] = ' ';

           uintToBase( reg[i][8],auxBuffer,  16);

           for(int j = 0;auxBuffer[j] != '\0'; j++){
               result[counter++] = auxBuffer[j];
           }
                       result[counter++] = ' ';

           char * low = "Lowest";
           char * med = "Medium";
           char * high = "Highest";
           char * shellp = "ShellPrio";

           if (tasks[i].priority == LOWEST){
               for (int k = 0; low[k]!='\0'; ++k) {
                   result[counter++] = low[k];
               }
           }
           else if (tasks[i].priority==MEDIUM){
               for (int k = 0; med[k]!='\0'; ++k) {
                   result[counter++] = med[k];
               }
           }
           else if(tasks[i].priority==HIGHEST){
                for (int k = 0; high[k]!='\0'; ++k) {
                   result[counter++] = high[k];
               }
           }else{
            for (int k = 0; high[k]!='\0'; ++k) {
                   result[counter++] = shellp[k];
               }
           }
           result[counter++] = ' ';
           char * ready = "Ready";
           char * blocked = "Blocked";
           if (tasks[i].status == READY){
               for (int k = 0; ready[k]!='\0'; ++k) {
                   result[counter++] = ready[k];
               }
           }
           else{
               for (int k = 0; blocked[k]!='\0'; ++k) {
                   result[counter++] = blocked[k];
               }
           }
           result[counter++] = '\n';
       }
   }
   result[counter] = '\0';
}

int add_task( void * task,int ground,int priority,uint64_t argc, char * argv[], uint64_t flags){

    for (int i = 0; i < MAX_TASKS; ++i) {
        if(tasks[i].present!=1 || tasks[i].status == KILLED){
            tasks[i].func=task;
            tasks[i].present = 1;
            tasks[i].status = READY;
            tasks[i].pID = pIDCounter++;
            tasks[i].name = argv[0];
            reg[i][0]= (uint64_t) tasks[i].func;
            reg[i][5] = (uint64_t)argv;
            reg[i][6]= argc;
            reg[i][8]= (uint64_t)(stack[i]+4095);
            reg[i][17]=flags;
            tasks[i].argc = argc;
            tasks[i].argv=argv;
            tasks[i].priority = priority;
            tasks[i].ground = ground;
            processes++;
            // ncPrint(argv[0]);
            return tasks[i].pID;
        }
    }
    return -1;
}


uint64_t * getRegisters(){
    next();
    return reg[activePID];
}

void next(){
    if (tasks[activePID].present==1 && tasks[activePID].status == KILLED){
        tasks[activePID].present =0;
    }
    else if (tasks[activePID].priority == HIGHEST && priorityTickers[HIGHEST] < HIGHESTTICKS && tasks[activePID].status == READY){
        priorityTickers[HIGHEST]++;
        return;
    }
    else if (tasks[activePID].priority == MEDIUM && priorityTickers[MEDIUM] < MEDIUMTICKS && tasks[activePID].status == READY){
        priorityTickers[MEDIUM]++;
        return;
    }

    if (tasks[activePID].priority != SHELLPRIO)
        priorityTickers[tasks[activePID].priority] = 0;

    for (int i = activePID +1 ; i < activePID+MAX_TASKS; i++) {
        int j = i;
        if(i>=MAX_TASKS){
            j=i%MAX_TASKS;
        }
        if (tasks[j].present == 1 && tasks[j].status == READY && tasks[j].priority != SHELLPRIO) {
            if (j != activePID) {
                activePID = j;
                priorityTickers[tasks[activePID].priority]++;
                return;
            }
        }
        else if (tasks[j].present == 1 && tasks[j].status == READY && tasks[j].priority == SHELLPRIO && foregroundRunning() == 0)
        {
            if( foregroundRunning2() == 1){
                activePID = 1;
                return;
            }
            activePID=0;
            return;
        }

    }
}

int foregroundRunning(){
    for (int i = 0; i < MAX_TASKS; ++i) {
        if (tasks[i].present == 1 && tasks[i].status == READY && tasks[i].priority != SHELLPRIO && tasks[i].ground == FOREGROUND)
            return 1;
    }
    return 0;
}

int foregroundRunning2(){
    for (int i = 0; i < MAX_TASKS; ++i) {
        if (tasks[i].present == 1 && tasks[i].status != KILLED && tasks[i].priority != SHELLPRIO && tasks[i].ground == FOREGROUND){
            return 1;
        }

    }
    return 0;
}



int getPID(){
    return tasks[activePID].pID;
}


void exitCurrent(){
    if( activePID == 0){
        return;
    }
    tasks[activePID].status = KILLED;
    processes--;
}

int killProcess(int pid){
    if( pid == 0){
        return -1;
    }
    for( int i = 0 ; i < MAX_TASKS ; i++){
        if(tasks[i].present == 1 && tasks[i].pID == pid){
            tasks[i].status = KILLED;
            return 1;
        }
    }
    return -1;
}

void nice(int pid, int priority){
    if( pid == 0 || !(priority >= 0 && priority <= 2)){
        return;
    }
    for( int i = 0 ; i < MAX_TASKS ; i++){
        if(tasks[i].pID == pid){
            if(tasks[i].present == 1){
                tasks[i].priority = priority;
            }else{
                return;
            }
        }
    }
}

int blockProcess(int pid){
    if( pid == 0){
        return -1;
    }
    for( int i = 0 ; i < MAX_TASKS ; i++){
        if(tasks[i].present == 1 && tasks[i].pID == pid && tasks[i].status != KILLED ){
            tasks[i].status = BLOCKED;
            return 1;
        }
    }
    return -1;
}

int blockProcessTick(int pid){
    if( pid == 0){
        return -1;
    }
    for( int i = 0 ; i < MAX_TASKS ; i++){
        if(tasks[i].present == 1 && tasks[i].pID == pid && tasks[i].status != KILLED ){
            tasks[i].status = BLOCKED;
            callTimerTick();
            return 1;
        }
    }
    return -1;
}

int unblockProcess(int pid){
    if( pid == 0){
        return -1;
    }
    for( int i = 0 ; i < MAX_TASKS ; i++){
        if(tasks[i].present == 1 && tasks[i].pID == pid && tasks[i].status != KILLED){
            tasks[i].status = READY;
            return 1;

        }
    }
    return -1;
}

void schedulerExit(){
    if( activePID == 0){
        return;
    }
    

    for (int i = 1; i < MAX_TASKS; ++i) {
        if(tasks[i].present==1){
            tasks[i].status = KILLED;
            processes--;
        }
    }
}

int getProcesses(){
    return processes;
}

uint64_t * registerManager(uint64_t * registers, uint8_t load){
    if (load){
        for(int i = 0 ; i < 19 ; i++){
            reg[activePID][i] = registers[i];
        }
    }
    return getRegisters();
}


int shellRunning(){
    return activePID==0;
}

int isForeground(){
    return tasks[activePID].ground == FOREGROUND;
}

uint64_t * yield(uint64_t * registers, uint8_t load){
    int aux = activePID;
    tasks[aux].present = 0;
    uint64_t * toRet = registerManager(registers,load);
    tasks[aux].present = 1;
    return toRet;
}
