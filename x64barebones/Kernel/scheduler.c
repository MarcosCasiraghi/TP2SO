#define MAX_TASKS 2
typedef void (*functionPointer)(void);
typedef struct{
    char * name;
    functionPointer func;
    int present;
}FunctionType;
static int hasTasks =0;
static FunctionType tasks[MAX_TASKS];

void add_task(char *name, void * task){
    int i =0;
    while(i<MAX_TASKS){
        if(tasks[i].present!=1){
            tasks[i].func=task;
            tasks[i].name=name;
            tasks[i].present = 1;
            return;
        }
        i++;
    }
    //si llega aca ya tengo dos tasks
}
void scheduler(void){

    void (*p)(void);
    while(1){
        for(int i=0;i<MAX_TASKS;i++){
            if(tasks[i].present==1){
                p=(void*)tasks[i].func;
                (*p)();
            }
        }
    }
    return;

}

int tasksRunning(){
    if (tasks[0].present == 0){
        return 0;
    }
    else if (tasks[1].present == 0 && tasks[0].present == 1)
        return 1;
    return 2;
}
