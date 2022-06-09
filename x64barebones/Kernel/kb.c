#include <kb.h>

#define LSHIFT 0x2A
#define RSHIFT 0x36
#define RELEASE 0x80

char kbBuffer[256];
int kbBufferPos=0;
int kbBufferConsume=0;
static int mayus=0;

static uint8_t scancodeLToAscii[] = {

        0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9',  '0', '-', '=',
        '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',  '[', ']',
        '\n',    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0, '*',
        0,  ' ',   0,   1/*f1*/,   2/*f2*/,   3/*f3*/,   4/*f4*/,   0,   
        0,   0,   0,   0,    0,   0,
        0,    0,   0,   0, '-',   0,   0,   0, '+',   0,   0,   0,    0,   0,
        0,    0,   0,   0,   0,   0
};

static uint8_t scancodeLToAsciiMayus[] = {

      0,   27, '!', '@', '#', '$', '%', '^', '&', '*', '(',  ')', '_', '+',
   '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',  '{', '}',
   '\n',    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
      0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',    0, '*',
      0,  ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,
      0,    0,   0,   0, '-',   0,   0,   0, '+',   0,   0,   0,    0,   0,
      0,    0,   0,   0,   0,   0

};

void getKey(){
    uint8_t myChar= kbFlag();
    if(scancodeLToAscii[myChar] == 27){//kill all with esc
        schedulerExit(2);
        return;
    }
    if(scancodeLToAscii[myChar] == 3){//kill left
        schedulerExit(3);
        return;
    }
    if(scancodeLToAscii[myChar] == 4){//kill right
        schedulerExit(4);
        return;
    }
    if (scancodeLToAscii[myChar] == '='){//load registers for inforeg
        loadRegisters();
    }
    if(scancodeLToAscii[myChar] == 1){//freeze left
        freeze(1);
        return;
    }
    if(scancodeLToAscii[myChar] == 2){//freeze right
        freeze(2);
        return;
    }
    if (kbBufferPos == 255)
        {
        kbBufferConsume = 0;
        kbBufferPos = 0;
        }
    if (kbBufferPos == 255)
        kbBufferPos = 0;
    if( myChar == LSHIFT || myChar == RSHIFT)
        mayus = 1;
    if( myChar == LSHIFT+RELEASE || myChar == RSHIFT+RELEASE )
        mayus = 0;
    if( myChar < RELEASE && myChar != 0x00){
        if(mayus)
            kbBuffer[kbBufferPos++] = scancodeLToAsciiMayus[myChar];
        else
            kbBuffer[kbBufferPos++] = scancodeLToAscii[myChar];
    }

}

char getKbBuffer(){
    if (kbBufferConsume<kbBufferPos){
        return kbBuffer[kbBufferConsume++];
    }
    return 0;
}
