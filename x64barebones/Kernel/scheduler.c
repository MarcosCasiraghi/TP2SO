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
#define FOREGROUND 0
#define BACKGROUND 1

typedef void (*functionPointer)(void);

typedef struct{
    char * name;
    functionPointer func;
    int present;
    int status;
    int pID;
    uint64_t param;
    int priority;
    int ground;
}FunctionType;

static int activePID = 0;
static FunctionType tasks[MAX_TASKS];

static int splitScreenMode=0;

static char stack[MAX_TASKS+1][STACK_SIZE] = {0};
static uint64_t reg[MAX_TASKS+1][REGISTERS] ={0};
static int processes=0;
static int pIDCounter = 0;

void ps(char * result){
   int counter = 0;
   for (int i = 0; i < MAX_TASKS; i++){
       if (tasks[i].present == 1 && tasks[i].status != KILLED){

           for(int j = 0;tasks[i].name[j] != '\0'; j++){
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


           uintToBase( reg[i][7],auxBuffer, 16); //TODO: imprime el stack base pointer

           for(int j = 0;auxBuffer[j] != '\0'; j++){
               result[counter++] = auxBuffer[j];
           }

           result[counter++] = ' ';

           uintToBase( reg[i][8],auxBuffer,  16); //TODO: imprime el stack pointer

           for(int j = 0;auxBuffer[j] != '\0'; j++){
               result[counter++] = auxBuffer[j];
           }

           result[counter++] = '\n';
       }
   }
   result[counter] = '\0';
}

void add_task(char *name, void * task,int ground,int priority,uint64_t parametro, uint64_t flags){

    for (int i = 0; i < MAX_TASKS; ++i) {
        if(tasks[i].present!=1){
            tasks[i].func=task;
            tasks[i].name=name;
            tasks[i].present = 1;
            tasks[i].status = READY;
            tasks[i].pID = pIDCounter++;
            reg[i][0]= tasks[i].func;
            reg[i][6]= parametro;
            reg[i][8]= (stack[i]+4095);
            reg[i][17]=flags;
            tasks[i].param=parametro;
            tasks[i].priority = priority;
            tasks[i].ground = ground;
            processes++;
            return;
        }
    }
}


uint64_t * getRegisters(){
    next();
    return reg[activePID];
}
int getParameter(){
    return tasks[activePID].param;
}

void next(){
    if (tasks[activePID].present==1 && tasks[activePID].status == KILLED){
        tasks[activePID].present =0;
        int maxPrioIndex = -1;
        int maxPrio = -1;
        for (int i = 0; i < MAX_TASKS; ++i) {
            if (tasks[i].present==1 && tasks[i].status==READY && (maxPrioIndex == -1 || tasks[i].priority > maxPrio)){
                maxPrio = tasks[i].priority;
                maxPrioIndex = i;
            }
        }
        activePID = maxPrioIndex;
        return;
    }
    
    for (int i = activePID +1 ; i < activePID+MAX_TASKS; i++) {
        int j = i;
        if(i>=MAX_TASKS){
            j=i%MAX_TASKS;
        }
        if (tasks[j].present == 1 && tasks[j].status == READY && tasks[j].priority <= tasks[activePID].priority) {
            if (j != activePID) {
            activePID = j;
            return;
            }
        }

    }
}


int getActivePId(){
    return activePID;
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

void killProcess(int pid){
    if( pid == 0){
        return;
    }
    for( int i = 0 ; i < MAX_TASKS ; i++){
        if(tasks[i].pID == pid){
            if(tasks[i].present == 1){
                tasks[i].status = KILLED;
            }else{
                return;
            }
        }
    }
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

void blockProcess(int pid){
    if( pid == 0){
        return;
    }
    for( int i = 0 ; i < MAX_TASKS ; i++){
        if(tasks[i].pID == pid){
            if(tasks[i].present == 1){
                if(tasks[i].status == READY)
                    tasks[i].status = BLOCKED;
                else if(tasks[i].status == BLOCKED)
                    tasks[i].status = READY;
            }else{
                return;
            }
        }
    }
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
