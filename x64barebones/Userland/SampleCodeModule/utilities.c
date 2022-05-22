#include <utilities.h>
#include <stdint.h>

void clear(){
    sys_clear();
}

void print(char * string,uint8_t fontColor, uint8_t backColor){
    sys_write(string, fontColor, backColor);
}

char getChar(){
    char c;
    while(1){
        if (sys_read(KEYBOARD_FD, &c, 1) != 0 ){
            return c;
        }
    }
}

// ver si se puede sacar el fd del scanf
void scanf(int fd, char * buffer, int maxlength){
    int size=0;
    char c;
    do {
        c=getChar();
        buffer[size]=c;
        size++;
    } while (size<maxlength-1 && c!='\n');
    buffer[size]='\0';
}
