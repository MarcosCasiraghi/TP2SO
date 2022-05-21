#include <syscalls.h>
#include <utilities.h>
#include <color.h>

#define BUFFER_LENGTH 30

//char * v = (char*)0xB8000 + 79 * 2;

//static int var1 = 0;
//static int var2 = 0;


int main() {
	clear();
	print("Que modulo desea correr?", GREEN, BLACK);

	print("$", GREEN, BLACK);


	while(1){
		//char readBuffer[BUFFER_LENGTH]={0};
		//en utilitites voy a tener que hacer un par de cosas

	}

	/*//All the following code may be removed
	*v = 'X';
	*(v+1) = 0x74;


	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;*/
}
