#include <utilities.h>
#include <stdint.h>
#include <standardlib.h>

#define FULL_SCREEN 0
#define LEFT 1
#define RIGHT 2

void print(char * string,uint8_t fontColor, uint8_t backColor){
    sys_write(string, fontColor, backColor, strlen(string));
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
