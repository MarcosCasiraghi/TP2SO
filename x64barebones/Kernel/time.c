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

