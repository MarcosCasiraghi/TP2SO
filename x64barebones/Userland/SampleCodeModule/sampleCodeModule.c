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
	print("Bienvenido a la egipci0S \n", GREEN, BLACK,0);
	my_printf(0,"_  O        _  O         _  O        _  O\n");
 	my_printf(0," \\-|-\\_      \\-|-\\_       \\-|-\\_      \\-|-\\_\n");
	my_printf(0,"  /^\\         /^\\          /^\\         /^\\\n");
 	my_printf(0," ^^ ^^       ^^ ^^        ^^ ^^       ^^ ^^\n");

	//while(1){
	initShell();
	while(1){};
		//cuando limpiar buffer, esta lleno?
	//}
	return 0;
}
