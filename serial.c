#include <reg51.h>
#include "serial.h"


unsigned char key;


void setup_serial()
{
	TMOD = (TMOD & 0x0F) | 0x20;
	SCON = 0x50;
	TH1 = -3;
	TR1 = 1;	
}

void __putchar(unsigned char ch)
{
		SBUF = ch;
		while(TI == 0);
		TI = 0;
}

unsigned char __getchar()
{
		unsigned char ch;
	  while(RI == 0);
	  ch = SBUF;
	  RI = 0;
	  return ch;
}

void printString(const char *s)
{
		while(*s)
			__putchar(*(s++));	
}

unsigned long myAtoi(char *str)
{
    // Initialize result
    unsigned long res = 0;
    int i;
    for ( i = 0; str[i] != '\0'; i++)
        res = (res * 10) + (str[i] - '0');
 
    // return result.
    return res;
}

unsigned long  GetValue( unsigned char max_len)
{
	 // maximum of 6 digits per value
	 char arr[7] =""; 
   unsigned long  value = 0;
   char index ;

unsigned char *ptr;	

	      index = -1;
				ptr = arr;
				do
				{
						key = __getchar();			            
						__putchar(key);
					if(key < '0' || key > '9') { __putchar(8) ;    continue; } 
					if(key == 0x0D)  continue;
					
					*ptr = key ;		
					ptr++;	index++;		
          if( index == (max_len-1)) key = 0x0D;											
			 }while(key != 0x0D);
			 __putchar('\r');
			 __putchar('\n');
			 arr[index+1] = '\0';
			 value =  myAtoi( arr);				 
       return value;
}