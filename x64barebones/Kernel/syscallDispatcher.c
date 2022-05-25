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
    for (int i = 0; buffer[i]; i++){
        restoreDefault();
        if( buffer[i] == '\n'){
            ncNewline();
        }
        else if(buffer[i] == '\b'){
            backspace();
        }
        else
            ncPrintCharWithAtt(buffer[i], fontColor);
    }
}


void int_81(int fd, char * buffer){
    buffer[0]=getKbBuffer();
}

void int_82(){
    ncClear();
}

void int_83(int number){
    ncPrintDec(number);
    ncNewline();
}
