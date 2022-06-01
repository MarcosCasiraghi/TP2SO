#include <syscalls.h>
#include <utilities.h>
#include <color.h>
#include <fibonacci.h>
#include <shell.h>
#include <standardlib.h>

#define KEYBOARD_FD 1
#define SCREEN_FD 0

int main() {
	clear();
	print("Que modulo desea correr? \n", GREEN, BLACK,0);
	my_printf(0,"hola %dcomo te va \n",7);

	//while(1){

        shell();
		//cuando limpiar buffer, esta lleno?
	//}
	return 0;
}
