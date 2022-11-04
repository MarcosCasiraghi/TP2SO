#include <standardlib.h>
#include <utilities.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2


int my_strcmp(char * str1, char * str2){
    int i = 0 ;
    for( ; str1[i] && str2[i]; i++){
         if(str1[i] != str2[i])
            return 1;
    }
    if( str1[i] || str2[i] )
        return 2;
    return 0;
}

void put_char(char c){
  sys_write(&c, WHITE,BLACK, 1);
}

void sprint(char * str){
  sys_write(str, WHITE, BLACK, my_strlen(str));
}

int my_strlen(char * string){
  int length = 0;
  while(string[length] != '\0')
    length++;
  return length;
}

char getChar(){
    char c = 0;
    while( c == 0 ){
        sys_read(&c);
    }
    return c;
}

void my_scanf(char * buffer, int maxlength){
    int size=0;
    char c;
    do {
        if ((c=getChar())!='\n'){
            if (c=='\b' && size>0){
                size--;
            }else if(c!='\b'){
                buffer[size]=c;
                size++;
            }
            put_char(c);
        }
    } while (size<maxlength-1 && c != '\n');
    put_char('\n');
    buffer[size] = 0;
}

//Esta funcion la adaptados del siguiente link
// https://stackoverflow.com/questions/1735236/how-to-write-my-own-printf-in-c
void my_printf(const char * frmt, ...){
  // Module 1: Initializing Myprintf's arguments using stdarg.h
  va_list arg;   // declares a variable which we use to manipulating the argument list contaning variable arugments
  va_start(arg, frmt);   //initialize arg with function's last fixed argument, i.e. format

  const char *aux;

  uint64_t i;
  unsigned u;
  char *s;

  for(aux=frmt; *aux != '\0'; aux++){
    while(*aux != '%'){
      if(*aux == '\0'){
        va_end(arg);
        return;
      }
      put_char(*aux);
      aux++;
    }
    aux++;

    // Module 2: Fetching and executing arguments
    // va_arg() fetches the next argument from the argument list, where the 2nd parameter is the data type expected
    // ONLY accept char*, unsigned int, int or double
    switch(*aux){
      case 'c' :  i = va_arg(arg, int);  // Fetch char argument
                  put_char(i);
                  break;
      case 'd' :  i = va_arg(arg, int);   // Fetch Decimal/Integer argument
                  if(i < 0){
                    i = -i;
                    put_char('-');
                  }
                  sprint(convert(i,10));
                  break;
      case 'o':   i = va_arg(arg, unsigned int);   // Fetch Octal representation
                  sprint(convert(i,8));
                  break;
      case 's':   s = va_arg(arg, char *);   // Fetch string
                  sprint(s);
                  break;
      case 'u':   u = va_arg(arg, unsigned int);   // Fetch Unsigned decimal integer
                  sprint(convert(u,10));
                  break;
      case 'x':   u = va_arg(arg, unsigned int);   // Fetch Hexadecimal representation
                  sprint(convert(u,16));
                  break;
      case '%':   put_char('%');
                  break;
    }
  }
  //Module 3: Closing argument list to necessary clean-up
  va_end(arg);
}

char *convert(unsigned int num, int base){
  static char Representation[] = "0123456789ABCDEF";
  static char buff[33];
  char *ptr;

  ptr = &buff[sizeof(buff)-1];
  *ptr = '\0';

  do{
    *--ptr = Representation[num % base];
    num /= base;
  }while(num != 0);

  return (ptr);
}

int atoi(const char * S){
    long num = 0;
 
    int i = 0;
    int negative = 0;
    int number = 0;
    // run till the end of the string is reached, or the
    // current character is non-numeric
    if( S[0] == '-'){
      negative = 1;
      i = 1;
    }
    while (S[i] && (S[i] >= '0' && S[i] <= '9'))
    {
        number = 1;
        num = num * 10 + (S[i] - '0');
        i++;
    }
    if(negative){
      return -num;
    }
    if(!number){
      return NULL;
    }
 
    return num;
}