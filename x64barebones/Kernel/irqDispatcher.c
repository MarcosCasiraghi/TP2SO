#include <irqDispatcher.h>

static void int_20();
static void int_21();

char kbBuffer[256];
int kbBufferPos=0;
int kbBufferConsume=0;

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
    //restoreDefault();
    char c = getKey();
    //if (c=='\b' && kbBufferPos!=0){
    //    kbBufferPos--;
     //   kbBufferConsume--;

    //   kbBufferPos-=2;
    //    kbBufferConsume--;
       // kbBuffer[kbBufferPos++]=c;
   // }
    kbBuffer[kbBufferPos++]=c;

    // if( c != 0){
    //     if(c == '\n')
    //         ncNewline();
    //     else if(c == '\b')
    //         backspace();
    //     else
    //         ncPrintCharWithAtt(c, LGREY);
    // }
}

char getKbBuffer(){
    if (kbBufferConsume<kbBufferPos){
        return kbBuffer[kbBufferConsume++];
    }
    return 0;
}

