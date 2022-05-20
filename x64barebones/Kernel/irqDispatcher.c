#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>

static void int_20();
static void int_60();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 2:
			int_60();
			break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_60(){
	ncPrint("hola mundo");
}
