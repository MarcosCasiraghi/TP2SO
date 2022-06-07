
#include <help.h>

void help(){
    print("HELP",WHITE,BLACK);
    print("   Comandos disponibles\n", MAGENTA, BLACK);
    print("- fibonacci: imprime la serie de fibonacci hasta que se corte su ejecucion\n",WHITE,BLACK);
    print("- primos: imprime los numeros primos hasta que se corte su ejecucion\n",WHITE,BLACK);
    print("- time: imprime el dia y la hora del sistema\n",WHITE,BLACK);
    print("- inforeg: imprime el valor de los registros si la tecla especial fue presionada\n",WHITE,BLACK);
    print("- printmem: realiza un vuelco de memoria de 32 bytes a partir de la direccion recibida como argumento\n",WHITE,BLACK);
    print("div0: verifica excepcion de division por 0\n",WHITE,BLACK);
    print("invalidopcode: verifica funcionameiento de excepcion de invalid opcode\n",WHITE,BLACK);
    exit();
}
