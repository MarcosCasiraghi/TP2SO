#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <time.h>
#include <kb.h>
#include <color.h>

static void int_20();
static void int_21();

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
    if( seconds_elapsed()%2 == 0)
        blink(0x0F);
    else
        blink(0x00);

}

void int_21(){
    restoreDefault();
    char c= getKey();
    if( c != 0){
        if(c == '\n')
            ncNewline();
        else if(c == '\b')
            backspace();
        else
            ncPrintCharWithAtt(c, LGREY);
    }
}


