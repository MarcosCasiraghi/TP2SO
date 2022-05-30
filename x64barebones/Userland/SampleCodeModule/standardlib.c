#include <standardlib.h>
#include <utilities.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

static char buffer[64]={'0'};


int strcmp(char * str1, char * str2){
    int i = 0 ;
    for( ; str1[i] && str2[i]; i++){
         if(str1[i] != str2[i])
            return 1;
    }
    if( str1[i] || str2[i] )
        return 2;
    return 0;
}

void put_char(uint8_t fd, char c){
  sys_write(&c, LGREY,BLACK, 1);
}

void sprint(uint8_t fd, char * str){
  sys_write(str, LGREY, BLACK, strlen(str));
}

int strlen(char * string){
  int length = 0;
  while(string[length] != '\0')
    length++;
  return length;
}

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
      put_char(STDOUT, *aux);
      aux++;
    }
    aux++;

    // Module 2: Fetching and executing arguments
    // va_arg() fetches the next argument from the argument list, where the 2nd parameter is the data type expected
    // ONLY accept char*, unsigned int, int or double
    switch(*aux){
      case 'c' :  i = va_arg(arg, int);  // Fetch char argument
                  put_char(STDOUT, i);
                  break;
      case 'd' :  i = va_arg(arg, int);   // Fetch Decimal/Integer argument
                  if(i < 0){
                    i = -i;
                    put_char(STDOUT, '-');
                  }
                  sprint(1, convert(i,10));
                  //uintToBase(i,buffer,10);
                  //sprint(1, buffer);
                  break;
      case 'o':   i = va_arg(arg, unsigned int);   // Fetch Octal representation
                  sprint(1, convert(i,8));
                  break;
      case 's':   s = va_arg(arg, char *);   // Fetch string
                  sprint(1, s);
                  break;
      case 'u':   u = va_arg(arg, unsigned int);   // Fetch Unsigned decimal integer
                  sprint(1, convert(u,10));
                  break;
      case 'x':   u = va_arg(arg, unsigned int);   // Fetch Hexadecimal representation
                  sprint(1, convert(u,16));
                  break;
      case '%':   put_char(STDOUT, '%');
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

// uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
// {
// 	char *p = buffer;
// 	char *p1, *p2;
// 	uint32_t digits = 0;

// 	//Calculate characters for each digit
// 	do
// 	{
// 		uint32_t remainder = value % base;
// 		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
// 		digits++;
// 	}
// 	while (value /= base);

// 	// Terminate string in buffer.
// 	*p = 0;

// 	//Reverse string in buffer.
// 	p1 = buffer;
// 	p2 = p - 1;
// 	while (p1 < p2)
// 	{
// 		char tmp = *p1;
// 		*p1 = *p2;
// 		*p2 = tmp;
// 		p1++;
// 		p2--;
// 	}

// 	return digits;
// }


// void my_printf(char* format,...)
// {
//     char *traverse;
//     unsigned int i;
//     char *s;

//     //Module 1: Initializing Myprintf's arguments
//     va_list arg;
//     va_start(arg, format);

//     for(traverse = format; *traverse != '\0'; traverse++)
//     {
//         while( *traverse != '%' )
//         {
//             put_char(1,*traverse);
//             traverse++;
//         }

//         traverse++;

//         //Module 2: Fetching and executing arguments
//         switch(*traverse)
//         {
//             case 'c' : i = va_arg(arg,int);     //Fetch char argument
//                         put_char(1,i);
//                         break;

//             case 'd' : i = va_arg(arg,int);         //Fetch Decimal/Integer argument
//                         if(i<0)
//                         {
//                             i = -i;
//                             put_char(1,'-');
//                         }
//                         sprint(1,convert(i,10));
//                         break;

//             case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
//                         ///puts(convert(i,8));
//                         break;

//             case 's': s = va_arg(arg,char *);       //Fetch string
//                        // puts(s);
//                         break;

//             case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
//                        // puts(convert(i,16));
//                         break;
//         }
//     }

//     //Module 3: Closing argument list to necessary clean-up
//     va_end(arg);
// }

// char *convert(unsigned int num, int base)
// {
//     static char Representation[]= "0123456789ABCDEF";
//     static char buffer[50];
//     char *ptr;

//     ptr = &buffer[49];
//     *ptr = '\0';

//     do
//     {
//         *--ptr = Representation[num%base];
//         num /= base;
//     }while(num != 0);

//     return(ptr);
// }
