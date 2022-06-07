
#include <help.h>

void help(){
    print("HELP",WHITE,YELLOW);
    print("   Comandos disponibles\n", BLUE, YELLOW);
    print("- fibonacci: imprime la serie de fibonacci hasta que se corte su ejecucion\n",WHITE,BLACK);
    print("- primos: imprime los numeros primos hasta que se corte su ejecucion\n",WHITE,BLACK);
    print("- time: imprime el dia y la hora del sistema\n",WHITE,BLACK);
    print("- inforeg: imprime el valor de todos los registros si la tecla especial fue presionada\n",WHITE,BLACK);
    print("- printmem: realiza un volcado de memoria de 32 bytes a partir de la direccion recibida como argumento\n",WHITE,BLACK);
    print("Teclas Especiales:\nESC: Mata la ejecucion de los programas activos\n",WHITE, BLACK);
    print("'=': Guarda estado actual de los registros para inforeg\n", WHITE, BLACK);
    print("'[' y ']': Mata la ejecucion del programa izquierdo y derecho\n", WHITE, BLACK);
    print("'f' y 'g': Freezea la ejecucion del programa izquierdo y derecho\n",WHITE,BLACK);
    exit();
}
