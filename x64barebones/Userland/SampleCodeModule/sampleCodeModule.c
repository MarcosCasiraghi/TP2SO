#include <syscalls.h>
#include <utilities.h>
#include <color.h>
#include <fibonacci.h>
#include <shell.h>
#include <standardlib.h>

#define BUFFER_LENGTH 50
#define KEYBOARD_FD 1
#define SCREEN_FD 0

int main() {
	clear();
	print("Que modulo desea correr? \n", GREEN, BLACK);
	my_printf("hola %dcomo te va \n",7);

	while(1){

		print("~$ ",GREEN, BLACK);
		char readBuffer[BUFFER_LENGTH]={0};
		scanf(KEYBOARD_FD, readBuffer, BUFFER_LENGTH);
		run(readBuffer);
		//cuando limpiar buffer, esta lleno?
	}
	return 0;
}
