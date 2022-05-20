#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>

static void int_20();
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
        case 2:
            int_80();
            break;
		case 3:
			int_81();
			break;
        case 4:
            int_82();
            break;
    }
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
    //Obtiene e imprime
    char c = getKey();
    if (c > 0x00){
        ncPrint(c);
    }
}

void int_80(){
	ncPrint("hola mundo");
}

void int_81(){

}

void int_82(){
    ncClear();
}

