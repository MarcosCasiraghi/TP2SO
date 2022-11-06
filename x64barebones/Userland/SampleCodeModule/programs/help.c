// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <help.h>

void help(){
    clear();
    print("HELP",WHITE,BLACK);
    print("   Comandos disponibles\n", MAGENTA, BLACK);
    print("- fibonacci: imprime la serie de fibonacci hasta que se corte su ejecucion\n",WHITE,BLACK);
    print("- primos: imprime los numeros primos hasta que se corte su ejecucion\n",WHITE,BLACK);
    print("- time: imprime el dia y la hora del sistema\n",WHITE,BLACK);
    print("- inforeg: imprime el valor de los registros si la tecla especial fue presionada",WHITE,BLACK);
    print("- printmem: realiza un vuelco de memoria de 32 bytes a partir de la direccion recibida como argumento\n",WHITE,BLACK);
    print("- div0: verifica excepcion de division por 0\n",WHITE,BLACK);
    print("- ps: imprime todos los procesos corriendose con su informacion\n",WHITE,BLACK);
    print("- clear: limpia la consola\n",WHITE,BLACK);
    print("- mmtest: corre el test de memory manager\n",WHITE,BLACK);
    print("- mmstatus: muestra el estado de la memoria\n",WHITE,BLACK);
    print("- processtest: corre el test de procesos\n",WHITE,BLACK);
    print("- semtest: corre el test de semaforos con los parametros cantidad de ciclos y un booleano para saber si usar semaforos\n",WHITE,BLACK);
    print("- semstatus: imprime el estado de los semaforos\n",WHITE,BLACK);
    print("- pipestatus: imprime el estado de los pipes\n",WHITE,BLACK);
    print("- phylo: corre e imprime el problema de los filosofos\n",WHITE,BLACK);
    print("- wc: imprime la cantidad de lineas del input\n",WHITE,BLACK);
    print("- filter: imprime el input sin vocales\n",WHITE,BLACK);
    print("- loop: imprime un mensaje con el pid del proceso cada una cantidad de segundos\n",WHITE,BLACK);
    print("- cat: imprime el input por salida estandar\n",WHITE,BLACK);
    print("- invalidopcode: verifica funcionameiento de excepcion de invalid opcode\n",WHITE,BLACK);

    exit();
 
    
}
