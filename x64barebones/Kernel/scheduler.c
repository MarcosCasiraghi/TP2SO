#include <scheduler.h>

#define STACK_SIZE 800
#define REGISTERS 20
#define MAX_TASKS 2
#define READY 0
#define HALTED 1
#define KILLED 2

typedef void (*functionPointer)(void);

typedef struct{
    char * name;
    functionPointer func;
    int present;
    int status;
    int pID;
}FunctionType;

static int hasTasks =0;
static int activePID = 0;
static FunctionType tasks[MAX_TASKS];

static char stack[3][STACK_SIZE] = {0};
static uint64_t reg[3][REGISTERS] ={0};
static int processes=0;

void add_task(char *name, void * task, uint64_t flags){
    //comente estas lineas para probar solo con scheduler
    //int i =0;
    // while(i<MAX_TASKS){
    //     if(tasks[i].present!=1){
    //         tasks[i].func=task;
    //         tasks[i].name=name;
    //         tasks[i].present = 1;
    //         tasks[i].status = READY;
    //         tasks[i].pID = i;

    //         //esto esta bien?
    //         reg[i][0]= tasks[i].func;
    //         reg[i][8]=stack[i];
    //         return;
    //     }
    //     i++;
    // }
        //si llega aca ya tengo dos tasks


            tasks[0].func=task;
            tasks[0].name=name;
            tasks[0].present = 1;
            tasks[0].status = READY;
            tasks[0].pID = 0;
            reg[0][0]= tasks[0].func;
            reg[0][8]= (stack[0]+799);
            reg[0][17]=flags;
            processes++;
            // puede que este aca
            return;

}

uint64_t * getRegisters(){
   // next();
    return reg[0];
}

void next(){
    if (tasks[1].status==READY){
        if (tasks[2].status==READY)
        {
            activePID= (activePID%2) + 1;
        }else
            activePID=1;
    }
}

int getProcesses(){
    return processes;
}

void setRegisters(uint64_t * registers, uint8_t load){
    if (load){
        for(int i = 0 ; i < 19 ; i++){
            reg[activePID][i] = registers[i];
        }
    }
}

int haltProcess(int pID){
    if (tasks[pID].present) {
        tasks[pID].status = HALTED;
        return 1;
    }
    return -1;
}

int killProcess(int pID){
    if (tasks[pID].present) {
        tasks[pID].status = KILLED;
        return 1;
    }
    return -1;
}

void * getTask(int pID){
    if( pID >= 0 && pID < 2){
        return tasks[pID].func;
    }
    return;
}

void * scheduler(void){

    void (*p)(void);
    // while(1){
    //     for(int i=0;i<MAX_TASKS;i++){
    //         if(tasks[i].present && tasks[i].status == READY){
    //             p=(void*)tasks[i].func;
    //             (*p)();
    //         }
    //     }
    // }
    activePID++;
    activePID %= 2;

    p=(void*)tasks[activePID].func;
    //(*p)();

    return p;

}

int tasksRunning(){
    if (tasks[0].present == 0){
        return 0;
    }
    else if (tasks[1].present == 0 && tasks[0].present == 1)
        return 1;
    return 2;
}

