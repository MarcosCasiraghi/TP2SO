#include <time.h>
#include <stdint.h>

static unsigned long ticks = 0;
static uint64_t reg1[32] = {0};
static uint64_t reg2[32] = {0};
static int activeTask = -1;

void timer_handler() {
	int taskNmbr = tasksRunning();
    if (taskNmbr == 2 && ticks_elapsed()%30 == 0){
        if (activeTask == -1){
           //ASM?
            void (*p)(void) = getTask(0);
            activeTask = 1;
            (*p)();
        }
        else if (activeTask == 1){
            //pushRegisters1(reg1);
            //task2(); //mismo problema q arriba si nunca se corrio el task2, sino ip ya estara seteado
            //popRegisters2(reg2);
            void (*p)(void) = getTask(1);
            activeTask = 2;
            (*p)();
        }
        else {//activeTask == 2
            //pushRegisters2(reg2);
            //popRegisters1(reg1);
            void (*p)(void) = getTask(0);
            activeTask = 1;
            (*p)();        
            }
    }
    else if(taskNmbr == 1){
        void (*p)(void) = getTask(0);
        (*p)();  
    }
    char c = getKbBuffer();
    if( c == 'u'){   //chequeo si es uns ESC; si lo es KILL a todos los procesos activos
        killProcess(0);
        killProcess(1);
    }
    ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
