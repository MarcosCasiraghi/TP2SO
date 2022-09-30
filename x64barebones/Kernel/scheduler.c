#include <scheduler.h>

#define STACK_SIZE 1600
#define REGISTERS 20
#define MAX_TASKS 2
#define READY 0
#define FROZEN 1
#define KILLED 2

typedef void (*functionPointer)(void);

typedef struct{
    char * name;
    functionPointer func;
    int present;
    int status;
    int pID;
    uint64_t param;
}FunctionType;

typedef struct FuntionTypeNode{
    FunctionType head;
    struct FunctionTypeNode * next;
}FunctionTypeNode;

typedef struct RegNode{
    uint64_t current[REGISTERS];
    struct RegNode * next;
}RegNode;

typedef struct StackNode{
    char current[STACK_SIZE];
    struct StackNode * next;
}StackNode;

static int activePID = 0;
//static FunctionType tasks[MAX_TASKS];
static FunctionTypeNode * first = NULL;
//TODO: hacer que el array tasks sea variable
//TODO: Hacer que los procesos tengan prioridad y hacer el next a partir de esto con round robin (carrousel) 

//TODO: funcion ps es recorrer tasks y listar lo que tiene
static int splitScreenMode=0;

//static char stack[MAX_TASKS+1][STACK_SIZE] = {0};
//static uint64_t reg[MAX_TASKS+1][REGISTERS] ={0};
static RegNode * rfirst = NULL;
static StackNode * sfirst = NULL;
static int processes=0;

//TODO agregar PID como parametro
//TODO esta memoria de cada proceso del scheduler, se deberia manejar con memory manager? preguntar a Martin
void add_task(char *name, void * task,uint64_t parametro, uint64_t flags, pid_t pid){
    FunctionTypeNode * current = first;
    RegNode * rcurrent = rfirst;
    StackNode * scurrent = sfirst;

    while(current != NULL){
        current = current->next;
        rcurrent = rcurrent->next;
        scurrent = scurrent->next;
    }
    current = malloc(sizeof(FunctionTypeNode));
    current->next=NULL;
    current->head.func=task;
    current->head.name=name;
    current->head.param=parametro;
    current->head.status = READY;
    current->head.pID = 0;

    scurrent = malloc(sizeof(StackNode));

    rcurrent = malloc(sizeof(RegNode));
    rcurrent->current[0] = first->head.func;
    rcurrent->current[6] = parametro;
    rcurrent->current[8] = sfirst->current+799;
    rcurrent->current[17]=flags;

    processes++;

}


uint64_t * getRegisters(){
    next();
    return reg[activePID];
}
int getParameter(){
    return tasks[activePID].param;
}

void next(){
    if(tasks[1].present==1 && tasks[1].status == KILLED && tasks[2].present==1 && tasks[2].status == KILLED){
        //Vuelve a shell y "elimina" funciones
        setCurrentVideo();
        splitScreenMode=0;
        activePID = 0;
        tasks[1].present = 0;
        tasks[2].present = 0;
        return;
    }

    if (tasks[1].present==1 && tasks[1].status == READY){
        if(tasks[2].present==1 && tasks[2].status == READY) //Si los 2 estan listos, cambia entre los 2
            activePID= (activePID%2) + 1;
        else
            activePID=1;
        return;
    }
    if (tasks[2].present==1 && tasks[2].status == READY && tasks[1].present==1 && tasks[1].status == KILLED ){
        activePID = 2;
        return;
    }

    if (!tasks[2].present && tasks[1].present && tasks[1].status==KILLED)
    {
        tasks[1].present=0;
        activePID = 0;
        return;
    }
    if(tasks[1].present && tasks[1].status == FROZEN && tasks[2].present && tasks[2].status == READY){
        activePID = 2;
        return;
    }
    if(tasks[2].present && tasks[2].status == FROZEN && tasks[1].present && tasks[1].status == READY){
        activePID = 1;
        return;
    }

    activePID=0;
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
    if( amountOfFuncs == 1){
        tasks[activePID].status = KILLED;
    }
    else if( amountOfFuncs == 2){
        tasks[1].present=0;
        tasks[2].present=0;
        splitScreenMode=0;
        ncClear();
    }
    else if( amountOfFuncs == 3){   //KILL left side
        if((tasks[1].present && tasks[2].present && tasks[2].status == READY) || tasks[2].status == KILLED){
            tasks[1].status = KILLED;
        }
    }
    else{   //KILL right side
        if((tasks[2].present && tasks[1].present && tasks[1].status == READY) || tasks[1].status == KILLED){
            tasks[2].status = KILLED;
        }
    }
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
    if ((tasks[1].present==1 && tasks[2].present==1) || splitScreenMode){
        return 2;
    }
    else if (tasks[1].present==1)
        return 1;
    return 0;
}

int shellRunning(){
    return activePID==0;
}

