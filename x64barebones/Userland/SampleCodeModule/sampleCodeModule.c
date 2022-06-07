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
	print("Bienvenido a \n", GREEN, BLACK);
	my_printf("                            ____   _____ \n");
 	my_printf("                           / __ \\ / ____|\n");
	my_printf("  _ __ ___   __ _ _ __ ___| |  | | (___  \n");
 	my_printf(" | '_ ` _ \\ / _` | '__/ __| |  | |\\___ \\ \n");
	my_printf(" | | | | | | (_| | | | (__| |__| |____) |\n");
	my_printf(" |_| |_| |_|\\__,_|_|  \\___|\\____/|_____/ \n");
	my_printf("\n");
	for(int i=0; i<555555550 ; i++){}
	clear();
	print("Que comando desea correr?\n", GREEN, BLACK);

	initShell();
	while(1){};
	return 0;
}
