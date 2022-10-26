#include <scheduler.h>

#define STACK_SIZE 1600
#define REGISTERS 20
#define MAX_TASKS 16
#define READY 0
#define FROZEN 1
#define KILLED 2
#define BUFFERSIZE 100
#define HIGHEST 0
#define MEDIUM 1
#define LOWEST 2

typedef void (*functionPointer)(void);

typedef struct{
    char * name;
    functionPointer func;
    int present;
    int status;
    int pID;
    uint64_t param;
    int priority;
}FunctionType;

static int activePID = 0;
static FunctionType tasks[MAX_TASKS];
//TODO: Hacer que los procesos tengan prioridad y hacer el next a partir de esto con round robin (carrousel)

static int splitScreenMode=0;

static char stack[MAX_TASKS+1][STACK_SIZE] = {0};
static uint64_t reg[MAX_TASKS+1][REGISTERS] ={0};
static int processes=0;

char* ps(){
    char result[BUFFERSIZE] = {'\0'};
   int j = 0;
   for (int i = 0; i < tasksRunning(); i++){
       if (tasks[i].present == 1){
           strcpy(result+j,  tasks[i].name);
           j+= strlen(tasks[i].name);
           char buffer[BUFFERSIZE]  = {0};
           itoa(tasks[i].pID, buffer, 10);
           strcpy(result+j, buffer);
           j+= strlen(buffer);
           itoa(tasks[i].priority, buffer, 10);
           strcpy(result+j, buffer);
           j+= strlen(buffer);

       }
   }
    return result;
}

void add_task(char *name, void * task,int priority,uint64_t parametro, uint64_t flags){

    for (int i = 0; i < MAX_TASKS; ++i) {
        if(tasks[i].present!=1){
            tasks[i].func=task;
            tasks[i].name=name;
            tasks[i].present = 1;
            tasks[i].status = READY;
            tasks[i].pID = 0;
            reg[i][0]= tasks[i].func;
            reg[i][6]= parametro;
            reg[i][8]= (stack[i]+799);
            reg[i][17]=flags;
            tasks[i].param=parametro;
            tasks[i].priority = priority;
            processes++;
            if (i==2){
                splitScreenMode=1;
                ncClear();
            }
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

    if (tasksReady() == 0){
        setCurrentVideo();
        activePID = 0;
        for (int i = 0; i < MAX_TASKS; ++i) {
            tasks[i].present = 0;
        }
        return;
    }


    if (tasks[activePID].status == KILLED){
        tasks[activePID].present =0;
        int maxPrioIndex = -1;
        int maxPrio = -1;
        for (int i = 0; i < MAX_TASKS; ++i) {
            if (maxPrioIndex == -1 || tasks[i].priority > maxPrio){
                maxPrio = tasks[i].priority;
                maxPrioIndex = i;
            }
        }
        activePID = maxPrioIndex;
        return;
    }


    for (int i = activePID +1 ; i < activePID+MAX_TASKS; i++) {
        int j;
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

void schedulerExit(int amountOfFuncs){
    if( activePID == 0)
        return;


    for (int i = 0; i < MAX_TASKS; ++i) {
        tasks[i].present = 0;
    }
    activePID = 0;

//
//    else if( amountOfFuncs == 3){   //KILL left side
//        if((tasks[1].present && tasks[2].present && tasks[2].status == READY) || tasks[2].status == KILLED){
//            tasks[1].status = KILLED;
//        }
//    }
//    else{   //KILL right side
//        if((tasks[2].present && tasks[1].present && tasks[1].status == READY) || tasks[1].status == KILLED){
//            tasks[2].status = KILLED;
//        }
//    }
}

//1 si se quiere freezear programa left
//2 si se quiere freezear programa right
void freeze(int func){
    if(func == 1){
        if(tasks[1].present == 1 && tasks[1].status==READY && tasks[2].present && tasks[2].status == READY){
            tasks[1].status = FROZEN;
        }else if(tasks[1].status == FROZEN){
            tasks[1].status = READY;
        }
    }else{
        if(tasks[2].present == 1 && tasks[2].status==READY && tasks[1].present && tasks[1].status == READY){
            tasks[2].status = FROZEN;
        }else if(tasks[2].status == FROZEN){
            tasks[2].status = READY;
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
        if (tasks[i].present == 1)
            runningCounter++;
    }
    return runningCounter;
}

int tasksReady(){
    int readyCounter = 0;
    for (int i = 0; i < MAX_TASKS; ++i) {
        if (tasks[i].status == READY)
            readyCounter++;
    }
    return readyCounter;
}

int shellRunning(){
    return activePID==0;
}

