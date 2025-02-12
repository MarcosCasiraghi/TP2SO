// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscallDispatcher.h>
#define KEYBOARD_FD 1
#define SCREEN_FD 0

static uint8_t hasRegDump=0;
static uint64_t regdump[17]={0};

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
	switch(r8){
		case 1:
			int_80((char *)rdi,rsi,rdx,rcx);
            break;
        case 2:
            int_81((char *)rdi);
            break;
        case 3:
            int_82();
            break;
        case 4:
            int_83(rdi);
            break;
        case 6:
            return int_85((uint64_t *)rdi);
            break;
        case 7:
            int_86((char *)rdi);
            break;
        case 8:
            int_87((uint8_t *)rdi, (char *) rsi);
            break;
        case 9:
            return (uint64_t) int_89(rdi);
        case 10:
            int_90((void *)rdi);
            break;
        case 11:
            int_91((char *)rdi);
            break;
        case 12:
            int_92((char *)rdi);
            break;
        case 13:
            int_94(rdi, rsi);
            break;
        case 14:
            return int_97(rdi, rsi);
            break;
        case 15:
            return int_98(rdi);
            break;
        case 16:
            return int_99(rdi);
            break;
        case 17:
            return int_100(rdi);
            break;
        case 18:
            int_101((char *)rdi);
            break;
        case 19:
            return int_102(rdi);
            break;
        case 20:
            return int_103(rdi);
            break;
        case 21:
            return int_104(rdi);
            break;
        case 22:
            return int_105(rdi,(char *) rsi);
            break;
        case 23:
            int_106((char *)rdi);
            break;
        case 24:
            return int_107();
            break;
	}
    return -1;
}

int schedulerDispatcher( void * func,int ground,int priority, uint64_t argc, char * argv[],uint64_t flags){
    return add_task( func,ground,priority, argc, argv, flags);
}

//a lo mejor en el futuro haya que agregar cases o mas parametros para el split screen
void int_80(char * buffer, uint8_t fontColor, uint8_t backColor, int length) {
    if (isForeground() == 0){
        return;
    }
    for (int i = 0; i < length; i++) {
        restoreDefault();
        if (buffer[i] == '\n') {
                ncNewline();
        } else if (buffer[i] == '\b') {
            backspace();
        } else{
            ncPrintCharWithAtt(buffer[i], fontColor);
        }
    }
}


void int_81(char * buffer){
    buffer[0]=getKbBuffer();
}

void int_82(){
    ncClear();
}

//sys exit
void int_83(){
    exitCurrent();
}

void registersForInforeg(uint64_t * registers){
    for (int i = 0; i < 17; i++){
        regdump[i]=registers[i];
    }
    
    hasRegDump=1;
}

int int_85(uint64_t * registers){
    for (int i = 0; i < 17; i++){
        if(hasRegDump){
            for(uint8_t i=0; i<17; i++){
                registers[i] = regdump[i];
            }
        }
        registers[17]=hasRegDump;
    }
    return hasRegDump;
}

void int_86(char * buffer){
    int currentBuffPos = 0;

    char bufferD1[3] = {0};
    char bufferD2[3] = {0};
    //char bufferD3[3] = {0};
    char bufferH[3] = {0};
	char bufferM[3] = {0};
	char bufferS[3] = {0};

    uintToBase(getRTC(7), bufferD1, 16);
    if (bufferD1[1] == 0) {
        buffer[currentBuffPos++] = '0';
        buffer[currentBuffPos++] = bufferD1[0];
    }else{
        buffer[currentBuffPos++] = bufferD1[0];
        buffer[currentBuffPos++] = bufferD1[1];
    }

    buffer[currentBuffPos++] = '/';

    uintToBase(getRTC(8), bufferD2, 16);
    if (bufferD2[1] == 0) {
        buffer[currentBuffPos++] = '0';
        buffer[currentBuffPos++] = bufferD2[0];
    }else{
        buffer[currentBuffPos++] = bufferD2[0];
        buffer[currentBuffPos++] = bufferD2[1];
    }

    buffer[currentBuffPos++] = '/';

    uintToBase(getRTC(9), buffer+currentBuffPos, 16);
    currentBuffPos += 2;
    buffer[currentBuffPos++] = '\n';

    uintToBase(getRTC(4), bufferH, 16);
    if (bufferH[1] == 0){
        buffer[currentBuffPos++] = '0';
        buffer[currentBuffPos++] = bufferH[0];
    }else{
        buffer[currentBuffPos++] = bufferH[0];
        buffer[currentBuffPos++] = bufferH[1];
    }
    buffer[currentBuffPos++] = ':';

    uintToBase(getRTC(2), bufferM, 16);
    if (bufferM[1] == 0){
        buffer[currentBuffPos++] = '0';
        buffer[currentBuffPos++] = bufferM[0];
    }else{
        buffer[currentBuffPos++] = bufferM[0];
        buffer[currentBuffPos++] = bufferM[1];
    }
    buffer[currentBuffPos++] = ':';

    uintToBase(getRTC(0), bufferS, 16);
    if (bufferS[1] == 0){
        buffer[currentBuffPos++] = '0';
        buffer[currentBuffPos++] = bufferS[0];
    }else{
        buffer[currentBuffPos++] = bufferS[0];
        buffer[currentBuffPos++] = bufferS[1];
    }


}

void int_87(uint8_t * address, char * buffer){
    int j = 0;
    for(int i=0; i<32; i++) {
        buffer[j++] = '0';
        buffer[j++] = 'x';
		buffer[j++] = hexaChar(address[i] >> 4);
		buffer[j++] = hexaChar(address[i] & 0x0F);
        buffer[j++] = ' ';
	}
}

void * int_89(uint64_t size){
    return allocMemory(size);
}

void int_90(void * address){
    freeMemory(address);
}

void int_91(char * buffer){
    memoryDump(buffer);
}

void int_92(char * buffer){
    ps(buffer);
}

void int_94(int pid, int priority){
    nice(pid, priority);
}

int int_97(int id, int value){
    return semOpen(id, value);
}
int int_98(int id){
    return semClose(id);

}
int int_99(int id){
    return post(id);
}
int int_100(int id){
    return wait(id);
}
void int_101(char * buffer){
    semStatus(buffer);
}

int int_102(int pipeId){
    return openPipe(pipeId);
}

int int_103(int pipeId){
    return closePipe(pipeId);
}

int int_104(int pipeId){
    return readPipe(pipeId);
}

int int_105(int pipeId, char * string){
    return writePipe(pipeId, string);
}

void int_106(char * buffer){
    pipeStatus(buffer);
}

int int_107(){
    return seconds_elapsed();
}
