#include <exceptions.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 1

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID){
		char message[]="0 division exception";
		exceptionCall(message);
	}else if (exception==INVALID_OPCODE_EXCEPTION_ID)
	{
		char message[] = "Invalid opcode exception";
		exceptionCall(message);
	}

}

void exceptionCall(char * message) {
	int activePid=getActivePId();
	int i=0;

	if (activePid==1 && getSplitScreen()==1){
		while (message[i]!=0){
			restoreDefault();
			printCharLeft(message[i]);
			i++;
		}
		newLineLeft();
	}else if (activePid==1){
			restoreDefault();
			ncPrint(message);
			ncNewline();
	}else{
		while (message[i]!=0){
			printCharRight(message[i]);
			i++;
		}
		newLineRight();
	}
	schedulerExit(1);
	for (int i = 0; i < 50000000; i++){

	}
}
