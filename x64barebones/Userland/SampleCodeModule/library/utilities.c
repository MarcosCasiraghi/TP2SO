#include <utilities.h>
#include <stdint.h>
#include <standardlib.h>

#define FULL_SCREEN 0
#define LEFT 1
#define RIGHT 2
#define BUFFERSIZE 500
#define EOF -1

static int is_vowel(char c);
void print(char * string,uint8_t fontColor, uint8_t backColor){
    sys_write(string, fontColor, backColor, my_strlen(string));
}

void exit(){
    sys_exit();
}

void time(){
    char time[25] = {0};

    sys_time(time);
    my_printf("La Fecha y Hora actual del sistema es:\n");
    my_printf(time);
    my_printf("\n");
    exit();
}

void clear(){
    sys_clear();
}

void clearConsole()
{
    sys_clear();
    exit();
}

void ps(){
    my_printf("name PID type visibility RBP RSP Priority Status\n");
    char buffer[BUFFERSIZE] = {'\0'};
    sys_ps(buffer);
    my_printf(buffer);
    exit();
}

void kill(uint64_t argc, char ** argv){
    // my_printf("%d\n%s\n%s\n", argc, argv[0], argv[1] );
    if( argc != 2){
        my_printf("Esta funcion debe recibir un parametro\nEscribirlo de la forma 'kill <numero>'\n");
        exit();
    }
    for( int i = 0; argv[1][i] != '\0' ; i++){
        if( argv[1][i] < '0' || argv[1][i] > '9'){
            my_printf("El parametro de esta funcion debe ser un numero\n");
            exit();
        }
    }
    int PID = atoi(argv[1]);

    if( PID == 0 ){
        my_printf("No se puede matar al proceso Shell\n");
    }

    int killed = sys_kill(PID);
    if(killed == -1){
        my_printf("Proceso con PID: %d no pudo ser matado\n", PID);
    }
    else{
        my_printf("Proceso con PID: %d matado con exito\n", PID);
    }
    exit();
}

int pipeOpen(int pipeId){
    if (sys_pipeOpen(pipeId) == -1){
        return -1;
    }
    return pipeId;
}

int pipeClose(int pipeId){
    if (sys_pipeClose(pipeId) == -1){
        return -1;
    }
    return 1;
}

int pipeWrite(int pipeId, char * string){
    if (sys_pipeWrite(pipeId, string) == -1){
        return -1;
    }
    return 1;
}

char pipeRead(int pipeId){
    char c = sys_pipeRead(pipeId);
    if (c == '\t'){
        return -1;
    }
    return c;
}

void pipeStatus(char * buffer){
    sys_pipeStatus(buffer);
    my_printf(buffer);
    exit();
}



int semOpen(int id, int value){
    return sys_semOpen(id,value);
}

int semClose(int id){
    return sys_semClose(id);
}
int post(int id){
    sys_semPost(id);

}
int wait(int id){
    // my_printf("estoy por bloquearme %d\n", id);
    sys_semWait(id);
}
void semStatus(){
    char buffer[BUFFERSIZE] = {'\0'};
    sys_semStatus(buffer);
    my_printf(buffer);
    exit();
}
void mmStatus(){
    char buffer[BUFFERSIZE] = {'\0'};
    sys_memStatus(buffer);
    my_printf(buffer);
    exit();
}

void yield(){
    sys_yield();
}

void sleep(int secs){
    int wait = sys_getseconds() + secs;
    while (sys_getseconds() < wait){};
}

void loop(int argc, char **argv) {
 

  int pid = sys_getPID();
   if (argc ==3) {
    int pipeId= pipeOpen(atoi(argv[1]));
    while (1) {
        char * saludo = "Que hay de nuevo viejo. No es temporada de pid:";
        char * result= sys_malloc(100 * sizeof(char));
//        char buff[4] = {0};
//        itoa(pid,buff,10);
        int i =0;
        while(saludo[i]!='\0'){
            result[i]= saludo[i];
            i++;
        }
        int j =0;
//        while(buff[j]!='\0'){
//            result[i]= buff[j];
//            j++;
//            i++;
//        }
        result[i] = '\n';
        pipeWrite(pipeId,result);
//        my_printf("escribi %s en el pipe\n", result);
        sleep(2);
  }
  }
  else{
  while (1) {
    my_printf("Que hay de nuevo viejo. No es temporada de pid:%d\n", pid);
    sleep(2);
  }
  }
  exit();
}

void filter(int argc, char **argv) {
    char c;
    if (argc == 3) {
        sleep(3);
        int pipeId = pipeOpen(atoi(argv[1]));
        // my_printf("%d",atoi(argv[1]));

        // my_printf("%d", pipeId);
    while ((c = pipeRead(pipeId)) != EOF) {
       if (!is_vowel(c)) {
          put_char(c);
       }
    }
        my_printf("No encontro nada pra leer y se fue a la mierda\n");
        pipeClose(pipeId);
    }
    else {

        while ((c = getChar()) != EOF) {
            if (!is_vowel(c)) {
                put_char(c);
            }
        }
    }
    exit();
}


void cat(int argc, char **argv) {
  char c;
  if (argc == 3) {
    int pipeId= pipeOpen(atoi(argv[1]));
    while ((c = pipeRead(pipeId)) != EOF) {
        put_char(c);
    }
  pipeClose(pipeId);
  }else{
  
  while ((c = getChar()) != EOF) {
    put_char(c);
  }
  }
  exit();
}

void wc(int argc, char **argv) {
    char c;
    int lines = 1;
    if (argc == 3) {
        int pipeId= pipeOpen(atoi(argv[1]));
        while ((c = pipeRead(pipeId)) != EOF) {
        // put_char(c);
            if (c == '\n') {
                lines++;
                my_printf("\n\nCantidad de lineas: %d\n", lines);
            }
        }
        pipeClose(pipeId);
    }else{
        while ((c = getChar()) != EOF) {
            put_char(c);
            if (c == '\n') {
                lines++;
                my_printf("\n\nCantidad de lineas: %d\n", lines);
            }
        }
    }
    
    exit();
}

static int is_vowel(char c){
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c== 'A' || c == 'E'
        || c == 'I' || c=='O' || c == 'U';
}

void writer(){
    pipeOpen(2);
    pipeWrite(2,"HOLA");
    exit();
}
void reader(){
    pipeOpen(2);
    char c;
    while ((c = pipeRead(2)) != EOF){
        put_char(c);
    }
    exit();
}

void idleProcessShell(){
    while(1);
}