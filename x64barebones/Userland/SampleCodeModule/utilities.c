#include <utilities.h>
#include <stdint.h>
#include <standardlib.h>

#define FULL_SCREEN 0
#define LEFT 1
#define RIGHT 2

void print(char * string,uint8_t fontColor, uint8_t backColor){
    sys_write(string, fontColor, backColor, strlen(string));
}


char getChar(){
    char c = 0;
    while( c == 0 ){
        sys_read(&c);
    }
    return c;
}

// ver si se puede sacar el fd del scanf
void scanf(char * buffer, int maxlength){
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
            put_char(c);
        }
    } while (size<maxlength-1 && c != '\n');
    put_char('\n');
    buffer[size] = 0;
}

void exit(){
    sys_exit();
    for( int i = 0 ; i < 999999999 ; i++){}
}

void time(){
    char time[25] = {0};

    sys_time(time);
    my_printf("La Fecha y Hora actual del sistema es:\n");
    my_printf(time);
    my_printf("\n");
    exit();
}

void clear(){
    sys_clear();
}
