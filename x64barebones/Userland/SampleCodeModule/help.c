
#include <help.h>

void help(){
    print("HELP\n",RED,YELLOW);
    print("Comandos disponibles\n", BLUE, YELLOW);
    print("- fibonacci: imprime la serie de fibonacci hasta que se corte su ejecucion\n",LGREY,BLACK);
    print("- primos: imprime los numeros primos hasta que se corte su ejecucion\n",LGREY,BLACK);
    print("- time: imprime el dia y la hora del sistema\n",LGREY,BLACK);
    print("- inforeg: imprime el valor de todos los registros\n",LGREY,BLACK);
    print("- printmem: recibe un argumento y realiza en memoria un volcado de memoria de 32 bytes a partir de la direccion recibida como argumento\n",LGREY,BLACK);
}