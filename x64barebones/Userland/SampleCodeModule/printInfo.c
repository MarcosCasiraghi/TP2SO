#include <printInfo.h>

#define MAX_ADDRESS_DECIMAL 2147483616



int readAddress(char* s, uint8_t** result) {
	uint64_t len = strlen(s);
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
	my_printf(0, "%d\n", dir);
	if(dir > MAX_ADDRESS_DECIMAL)
		return 0;
	
	*result = (uint8_t*)dir;
	return 1;
}

void printMem(char param[20]) {
	if(param[0]==0){
		my_printf(0, "Esta funcion requiere un parametro\n");
		exit();
	}

	uint8_t* p;
	if (!readAddress(param, &p)) {
		my_printf(0,"El parametro ingresado no es valido\n");
		exit();
	}
	
	for(int i=0; i<32; i++) {
		char mem[6] = "0x00 ";
		mem[2] = hexaChar(p[i] >> 4);
		mem[3] = hexaChar(p[i] & 0x0F);
		mem[5]=0;
		print(mem,WHITE, BLACK, 0);
	}
	my_printf(0, "\n");

	exit();
}
void inforeg(){
    uint64_t reg[17]={0};
	char* nameReg[17]={"RIP:","RAX:","RBX:","RCX:","RDX:","RSI:",
					"RDI:","RBP:","RSP","R8:","R9:","R10:","R11:",
					"R12:","R13:","R14:","R15:"};


    if (sys_registers(reg)){
        for (int i = 0;i < 17; i++){
			my_printf(0,nameReg[i]);
            my_printf(0,"%d",reg[i]);
            print("\n",BLACK,BLACK, 0);
        }
    }

    exit();
}

char hexaChar(uint8_t value) {
	     return value >= 10 ? (value - 10 + 'A') : (value + '0');
}