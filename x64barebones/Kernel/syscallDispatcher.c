#include <syscallDispatcher.h>

#define KEYBOARD_FD 1
#define SCREEN_FD 0

void syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
	switch(r9){
		case 1:
			int_80(rdi,rsi,rdx,rcx, r8);
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
        case 6:
            int_85(rdi, rsi);
            break;
        case 7:
            int_86();
            break;

	}
}

void schedulerDispatcher(char * name, void * func, uint64_t parametro,uint64_t flags){
    add_task(name, func, parametro, flags);
}

//a lo mejor en el futuro haya que agregar cases o mas parametros para el split screen
void int_80(char * buffer, uint8_t fontColor, uint8_t backColor, int length,int fd) {
     int param;

    param=getActivePId();
    if(tasksRunning()==1){
        param=0;
    }
    for (int i = 0; i < length; i++) {
        restoreDefault();
        if (buffer[i] == '\n') {
            switch(param){
                case 0:
                    ncNewline();
                    break;
                case 1:
                    newLineLeft();
                    break;
                case 2:
                    newLineRight();
                    break;
            }
        } else if (buffer[i] == '\b') {
            backspace();
        } else if (param==0)
            ncPrintCharWithAtt(buffer[i], fontColor);
        else if (param == 1)
            printCharLeft(buffer[i]);
        else if (param == 2)
            printCharRight(buffer[i]);
    }
}


void int_81(int fd, char * buffer){
    buffer[0]=getKbBuffer();
}

void int_82(){
    ncClear();
}

void int_83(){
    schedulerExit(1);
}

void int_85(uint8_t fd, uint64_t * registers){
    ncPrint("Rax:");
    ncPrintDec(*registers);
    registers++;
    ncNewline();
    ncPrint("Rbx:");
    ncPrintDec(*registers);
    ncNewline();
}

void int_86(){
    printCurrentTime();
}
