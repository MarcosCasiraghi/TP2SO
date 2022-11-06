#include <kb.h>
#include <interrupts.h>

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
        0,  ' ',   0,   1/*f1*/,   2/*f2*/,   3/*f3*/,   4/*f4*/,   5/*f5*/,
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

//int getKey(uint64_t * registers){
int getKey(){
    uint8_t myChar= kbFlag();
    if(scancodeLToAscii[myChar] == 27){//kill all with esc
        freeAll();
        closeAllPipes();
        closeAllSem();
        schedulerExit(2);
        return 1;
    }
    if (scancodeLToAscii[myChar] == 5){//load registers for inforeg
        return 2;
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
    return 0;

}

char getKbBuffer(){
    if (kbBufferConsume<kbBufferPos){
        return kbBuffer[kbBufferConsume++];
    }
    return 0;
}
