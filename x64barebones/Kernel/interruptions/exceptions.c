// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <exceptions.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 1

static const char* tags[18] = {
     "RIP","RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15"
};


void exceptionDispatcher(int exception, uint64_t * registers) {
	if (exception == ZERO_EXCEPTION_ID){
		char message[]="0 division exception";
		exceptionCall(message, registers);
	}else if (exception==INVALID_OPCODE_EXCEPTION_ID)
	{
		char message[] = "Invalid opcode exception";
		exceptionCall(message, registers);
	}

}

void exceptionCall(char * message, uint64_t * registers) {
	restoreDefault();
	ncPrint(message);
	ncNewline();
	for (int i = 0; i < 17; i++){
		ncPrint(tags[i]);
		ncPrint(" ");
		ncPrintHex(registers[i]);
		ncNewline();
	}

	schedulerExit();
}
