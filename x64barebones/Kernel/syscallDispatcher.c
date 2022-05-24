#include <syscallDispatcher.h>

#define KEYBOARD_FD 1
#define SCREEN_FD 0

void syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx){
	switch(rcx){
		case 1:
			int_80(rdi,rsi,rdx);
            break;
        case 2:
            int_81(rdi,rsi);
            break;
        case 3:
            int_82();
            break;
        case 4:
            int_83(rdi);
            break;

	}
}

//a lo mejor en el futuro haya que agregar cases o mas parametros para el split screen
void int_80(char * buffer, uint8_t fontColor, uint8_t backColor){
	ncPrintWithAtt(buffer, fontColor, backColor);
    ncNewline();
}


void int_81(int fd, char * buffer){
    buffer[0]=getKbBuffer();
    if (buffer[0]!=0){
        restoreDefault();
        if( buffer[0] == '\n'){
            ncNewline();
        }
        else if(buffer[0] == '\b'){
            backspace();
        }
        else
            ncPrintCharWithAtt(buffer[0], LGREY);
    }
}

void int_82(){
    ncClear();
}

void int_83(int number){
    ncPrintDec(number);
    ncNewline();
}
