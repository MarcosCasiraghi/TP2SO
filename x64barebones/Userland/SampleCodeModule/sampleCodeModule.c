#include <syscalls.h>
#include <utilities.h>
#include <color.h>
#include <fibonacci.h>
#include <shell.h>

#define BUFFER_LENGTH 50
#define KEYBOARD_FD 1
#define SCREEN_FD 0

int main() {
	clear();
	print("Que modulo desea correr? ", GREEN, BLACK);

	while(1){
		char readBuffer[BUFFER_LENGTH]={0};
		scanf(KEYBOARD_FD, readBuffer, BUFFER_LENGTH);
		run(readBuffer);
		//fibonacci();
		//cuando limpiar buffer, esta lleno?
	}
	return 0;
}
