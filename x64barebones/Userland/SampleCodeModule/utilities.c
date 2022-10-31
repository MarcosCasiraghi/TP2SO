#include <utilities.h>
#include <stdint.h>
#include <standardlib.h>

#define FULL_SCREEN 0
#define LEFT 1
#define RIGHT 2
#define BUFFERSIZE 350


void print(char * string,uint8_t fontColor, uint8_t backColor){
    sys_write(string, fontColor, backColor, my_strlen(string));
}

void exit(){
    sys_exit();
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

void clearConsole()
{
    sys_clear();
    exit();
}

void ps(){
    my_printf("name PID priority visibility RBP RSP\n");
    char buffer[BUFFERSIZE] = {'\0'};
    sys_ps(buffer);
    my_printf(buffer);
    exit();
}

void kill(char * pid){
    if( *pid == '\0'){
        my_printf("Esta funcion debe recibir un parametro\nEscribirlo de la forma 'kill <numero>'\n");
        exit();
    }
    int PID = atoi(pid);

    if( PID == 0 ){
        my_printf("No se puede matar al proceso Shell\n");
    }

    sys_kill(PID);
    exit();
}


