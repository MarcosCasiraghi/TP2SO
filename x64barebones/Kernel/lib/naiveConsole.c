// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>

//static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '\0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static uint8_t XvideoPosition = 0, YvideoPosition=0;
static const uint32_t width = 80;
static const uint32_t height = 25;



//utilizado para cuando dejar de correr 2 programas 
//que terminan

void ncPrint(const char * string){
    int i;
    for (i = 0; string[i] != 0; i++)
        ncPrintChar(string[i]);
}


void ncPrintChar(char character)
{
    *currentVideo = character;
    currentVideo += 2;

    scrollUp();
}


void ncPrintWithAtt(const char * string, uint8_t fontColor, uint8_t backColor){
    int i;
    uint8_t att = ((backColor<<4)| fontColor);

    for (i = 0; string[i] != 0; i++)
        ncPrintCharWithAtt(string[i], att);
}


void ncPrintCharWithAtt(char character, uint8_t att){
    if (character=='\n'){
        ncNewline();
    }else{
        *currentVideo=character;
        *(currentVideo+1)=att;
        currentVideo+=2;
    }
    scrollUp();
}





void ncNewline(){
    do{
        ncPrintChar(' ');
    }
    while((uint64_t)(currentVideo - video) % (width * 2) != 0);
    YvideoPosition++;
    scrollUp();
}


void backspace(){
    if( currentVideo != video && (currentVideo - video) % (width*2) != 2*3){
        currentVideo-=2;
        *currentVideo = ' ';
    }
}

void blink(uint8_t backColor){
    uint8_t aux = *(currentVideo+1);
    aux &= 0x0F;
    aux |= (backColor << 4);
    *(currentVideo+1) = aux;
}


void scrollUp(){
    if( currentVideo - video >= 2*width*height){
        for( int i = 0 ; i <= (height) ; i++){
            for( int j = 0 ; j <= width; j++){
                video[2*(i*width+j)] = video[2*((i+1)*width + j)];
                video[2*(i*width+j)+1] = video[2*((i+1)*width + j)+1];
            }
        }
        currentVideo = currentVideo - 2*width;
    }
}


void restoreDefault(){
    *(currentVideo+1) = WHITE;
}


void ncPrintHex(uint64_t value)
{
    ncPrintBase(value, 16);
}


void ncPrintBase(uint64_t value, uint32_t base){
    restoreDefault();
    uintToBase(value, buffer, base);
    ncPrintWithAtt(buffer,WHITE,BLACK);
}

void ncClear()
{
    int i;

    for (i = 0; i < height * width; i++){
        video[i * 2] = ' ';
        video[i*2+1]=0x0F;
    }
    currentVideo = video;
    XvideoPosition = 0;
    YvideoPosition = 0;
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    //Calculate characters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }
    while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}
