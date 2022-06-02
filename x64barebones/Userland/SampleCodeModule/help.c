
#include <help.h>

void help(){
    print("HELP\n",RED,YELLOW,0);
    print("Comandos disponibles\n", BLUE, YELLOW,0);
    print("- fibonacci: imprime la serie de fibonacci hasta que se corte su ejecucion\n",LGREY,BLACK,0);
    print("- primos: imprime los numeros primos hasta que se corte su ejecucion\n",LGREY,BLACK,0);
    print("- time: imprime el dia y la hora del sistema\n",LGREY,BLACK,0);
    print("- inforeg: imprime el valor de todos los registros\n",LGREY,BLACK,0);
    print("- printmem: recibe un argumento y realiza un volcado de memoria de 32 bytes a partir de la direccion recibida como argumento\n",LGREY,BLACK,0);
    exit();
}
