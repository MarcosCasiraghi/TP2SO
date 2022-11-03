#include <printInfo.h>

#define MAX_ADDRESS_DECIMAL 2147483616

int readAddress(char* s, uint8_t** result) {
	uint64_t len = my_strlen(s);
	if(len<3 || len>10 || s[0]!='0' || s[1]!='x')
		return 0;

	uint64_t dir = 0;
	for(int i=2; i<len; i++){
		if(s[i]>='0' && s[i]<='9')
			dir = 16*dir + s[i]-'0';
		else if(s[i]>='a' && s[i]<='f')
			dir = 16*dir + s[i]-'a'+10;
		else
			return 0;
	}
	if(dir > MAX_ADDRESS_DECIMAL)
		return 0;

	*result = (uint8_t*)dir;
	return 1;
}

void printMem(uint64_t argc, char ** argv) {
	if(argc != 2){
		my_printf("Esta funcion requiere un parametro\n");
		exit();
	}


	uint8_t* p;
	if (!readAddress(argv[1], &p)) {
		my_printf("El parametro ingresado no es valido\n");
		exit();
	}

	char buffer[170] = {0};

	sys_getMem(p, buffer);

	print(buffer, WHITE, BLACK);
	print("\n",WHITE,BLACK);

	exit();
}
void inforeg(){
    uint64_t reg[18]={0};
	char* nameReg[17]={"RIP:","RAX:","RBX:","RCX:","RDX:","RSI:","RDI:","RBP:","RSP:","R8:","R9:","R10:","R11:","R12:","R13:","R14:","R15:"};

	sys_registers(reg);
    if (reg[17]){
        for (int i = 0;i < 17; i++){
			my_printf(nameReg[i]);
            my_printf("%d",reg[i]);
            print("\n",BLACK,BLACK);
        }
    }else{
		my_printf("Presione \"f5\" para snapshot\n");
	}

    exit();
}


