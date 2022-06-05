#include <utilities.h>
#include <stdint.h>
#include <standardlib.h>

#define FULL_SCREEN 0
#define LEFT 1
#define RIGHT 2

void clear(){
    sys_clear();
}


void print(char * string,uint8_t fontColor, uint8_t backColor, int fd){
    sys_write(string, fontColor, backColor, strlen(string), fd);
}


char getChar(){
    char c = 0;
    while( c == 0 ){
        sys_read(KEYBOARD_FD, &c);
    }
    return c;
}

// ver si se puede sacar el fd del scanf
void scanf(int fd, char * buffer, int maxlength){
    int size=0;
    char c;
    do {
        if ((c=getChar())!='\n'){
            if (c=='\b' && size>0){
                size--;
            }else if(c!='\b'){
                buffer[size]=c;
                size++;
            }
            put_char(0, c);
        }
    } while (size<maxlength-1 && c != '\n');
    put_char(0,'\n');
    buffer[size] = 0;
}

void exit(){
    sys_exit();
    for( int i = 0 ; i < 999999999 ; i++){}
}

void inforeg(){
    uint64_t reg[17]={0};


    if (sys_registers(reg)){
        for (int i = 0;i < 17; i++){
            my_printf(0,"%d",reg[i]);
            print("\n",BLACK,BLACK, 0);
        }
    }

    exit();
}

void time(){
    sys_time();
    exit();
}
