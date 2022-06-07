#include <time.h>
#include <stdint.h>

static unsigned long ticks = 0;

void timer_handler() {
    ticks++;
}
int tick_check(){
    return ticks%10==0;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void printCurrentTime(){
    char buffer[64] = { '0' }; 

    int activePid=getActivePId();
	int i=0;

    char * message = {"La Fecha y Hora actual del sistema es:"};
	if (activePid==1 && getSplitScreen()==1){
		restoreDefault();
        printLeft(message);
		newLineLeft();
        printTimeLeft();
		newLineLeft();
	}else if (activePid==1){
			restoreDefault();
			ncPrint(message);
			ncNewline();
			printTimeCenter();
			ncNewline();
	}else{
		restoreDefault();
		printRight(message);
		newLineRight();
		printTimeRight();
		newLineRight();
	}
}

void printTimeLeft(){
	char bufferDate[3] = {0};
	char bufferH[3] = {0};
	char bufferM[3] = {0};
	char bufferS[3] = {0};

	uintToBase(getRTC(7),bufferDate, 16);
	printLeft(bufferDate);
	printLeft("/");
	uintToBase(getRTC(8), bufferDate, 16);
	printLeft(bufferDate);
	printLeft("/");
	uintToBase(getRTC(9), bufferDate, 16);
	printLeft(bufferDate);
	newLineLeft();

	uintToBase(getRTC(4), bufferH, 16);
	if(bufferH[1] == 0)
		printLeft("0");
	printLeft(bufferH);
	printLeft(":");
	uintToBase(getRTC(2), bufferM, 16);
	if(bufferM[1] == 0)
		printLeft("0");
	printLeft(bufferM);
	printLeft(":");
	uintToBase(getRTC(0), bufferS, 16);
	if(bufferS[1] == 0)
		printLeft("0");
	printLeft(bufferS);
}

void printTimeRight(){
	char bufferDate[3] = {0};
	char bufferH[3] = {0};
	char bufferM[3] = {0};
	char bufferS[3] = {0};

	uintToBase(getRTC(7),bufferDate, 16);
	printRight(bufferDate);
	printRight("/");
	uintToBase(getRTC(8), bufferDate, 16);
	printRight(bufferDate);
	printRight("/");
	uintToBase(getRTC(9), bufferDate, 16);
	printRight(bufferDate);
	newLineRight();

	uintToBase(getRTC(4), bufferH, 16);
	if(bufferH[1] == 0)
		printRight("0");
	printRight(bufferH);
	printRight(":");
	uintToBase(getRTC(2), bufferM, 16);
	if(bufferM[1] == 0)
		printRight("0");
	printRight(bufferM);
	printRight(":");
	uintToBase(getRTC(0), bufferS, 16);
	if(bufferS[1] == 0)
		printRight("0");
	printRight(bufferS);
}

void printTimeCenter(){
	char bufferDate[3] = {0};
	char bufferH[3] = {0};
	char bufferM[3] = {0};
	char bufferS[3] = {0};

	uintToBase(getRTC(7),bufferDate, 16);
	ncPrint(bufferDate);
	ncPrint("/");
	uintToBase(getRTC(8), bufferDate, 16);
	ncPrint(bufferDate);
	ncPrint("/");
	uintToBase(getRTC(9), bufferDate, 16);
	ncPrint(bufferDate);
	ncNewline();

	uintToBase(getRTC(4), bufferH, 16);
	if(bufferH[1] == 0)
		ncPrint("0");
	ncPrint(bufferH);
	ncPrint(":");
	uintToBase(getRTC(2), bufferM, 16);
	if(bufferM[1] == 0)
		ncPrint("0");
	ncPrint(bufferM);
	ncPrint(":");
	uintToBase(getRTC(0), bufferS, 16);
	if(bufferS[1] == 0)
		ncPrint("0");
	ncPrint(bufferS);
}

