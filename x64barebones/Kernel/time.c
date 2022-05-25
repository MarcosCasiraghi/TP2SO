#include <time.h>
#include <stdint.h>

static unsigned long ticks = 0;
static uint64_t reg1[32] = {0};
static uint64_t reg2[32] = {0};
static int activeTask = -1;

void timer_handler() {
	int taskNmbr = tasksRunning();
    if (taskNmbr == 2){
        if (activeTask == -1){
            //task1();   //ASM? Como consigue el front el puntero a funcion de la task 1
            activeTask == 1;
        }
        else if (activeTask == 1){
            //pushRegisters1(reg1);
            //task2(); //mismo problema q arriba si nunca se corrio el task2, sino ip ya estara seteado
            //popRegisters2(reg2);
            activeTask = 2;
        }
        else {
            //pushRegisters2(reg2);
            //popRegisters1(reg1);
            activeTask = 1;
        }
    }
    ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
