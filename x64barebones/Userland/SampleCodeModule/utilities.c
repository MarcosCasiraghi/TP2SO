#include <utilities.h>
#include <stdint.h>

void clear(){
    sys_clear();
}

void print(char * string,uint8_t fontColor, uint8_t backColor){
    sys_write(string, fontColor, backColor);
}


