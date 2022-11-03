#include <irqDispatcher.h>

static void int_20();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
    }
	return;
}

void int_20() {
	timer_handler();
    if(shellRunning()){     //Blink para simular representacion de posicion actual del cursor
        if( seconds_elapsed()%2 == 0)
            blink(0x0F);
        else
            blink(0x00);
    }

}

