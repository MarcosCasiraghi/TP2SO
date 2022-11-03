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
#define FOREGROUND 0
#define BACKGROUND 1

#define HIGHESTTICKS 3
#define MEDIUMTICKS 2
#define LOWESTTICKS 1

typedef void (*functionPointer)(void);

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

static int splitScreenMode=0;

static char stack[MAX_TASKS+1][STACK_SIZE] = {0};
static uint64_t reg[MAX_TASKS+1][REGISTERS] ={0};
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

           result[counter++] = '\n';
       }
   }
   result[counter] = '\0';
}

int add_task( void * task,int ground,int priority,uint64_t argc, char * argv[], uint64_t flags){

    for (int i = 0; i < MAX_TASKS; ++i) {
        if(tasks[i].present!=1 || ( tasks[i].present == 1 && tasks[i].status == KILLED)){
            tasks[i].func=task;
            tasks[i].present = 1;
            tasks[i].status = READY;
            tasks[i].pID = pIDCounter++;
            tasks[i].name = argv[0];
            reg[i][0]= tasks[i].func;
            reg[i][5] = argv;
            reg[i][6]= argc;
            reg[i][8]= (stack[i]+4095);
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
    else if (tasks[activePID].priority == HIGHEST && priorityTickers[HIGHEST] < HIGHESTTICKS){
        priorityTickers[HIGHEST]++;
        return;
    }
    else if (tasks[activePID].priority == MEDIUM && priorityTickers[MEDIUM] < MEDIUMTICKS){
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
            activePID=0;
            return;
        }

    }
}

int foregroundRunning(){
    for (int i = 0; i < MAX_TASKS; ++i) {
        if (tasks[i].present == 1 && tasks[i].status != KILLED && tasks[i].priority != SHELLPRIO && tasks[i].ground == FOREGROUND)
            return 1;
    }
    return 0;
}


int getActivePId(){
    return activePID;
}

int getPID(){
    return tasks[activePID].pID;
}

int getSplitScreen(){
return splitScreenMode;
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
        if(tasks[i].present == 1 && tasks[i].pID == pid){
            if(tasks[i].status == READY){
                tasks[i].status = BLOCKED;
                return 1;
            }
            else if(tasks[i].status == BLOCKED){
                tasks[i].status = READY;
                return 2;
            }
        }
    }
    return -1;
}

void schedulerExit(int amountOfFuncs){
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

int tasksRunning(){
    int runningCounter = 0;
    for (int i = 0; i < MAX_TASKS; ++i) {
        if (tasks[i].present == 1 && tasks[i].status != KILLED)
            runningCounter++;
    }
    return runningCounter;
}

int tasksReady(){
    int readyCounter = 0;
    for (int i = 1; i < MAX_TASKS; ++i) {
        if (tasks[i].present == 1 && tasks[i].status == READY)
            readyCounter++;
    }
    return readyCounter;
}

int shellRunning(){
    return activePID==0;
}

int isForeground(){
    return tasks[activePID].ground == FOREGROUND;
}

void yield(uint64_t * registers, uint8_t load){
    int aux = activePID;
    tasks[aux].status = KILLED;
    registerManager(registers,load);
    tasks[aux].status = READY;
}
