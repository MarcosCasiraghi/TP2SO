#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <time.h>
#include <kb.h>

static void int_20();
static void int_21();
static void int_80();
static void int_81();
static void int_82();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
        case 1:
            int_21();
            break;
    }
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
    //Obtiene e imprime
    char c= getKey();
    if (c > 0x00){
        ncPrint(&c);
    }
}

//observar

void int_81(){

}

void int_82(){
    ncClear();
}

