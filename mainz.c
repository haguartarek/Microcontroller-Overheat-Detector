#include <stdlib.h>
#include <reg51.h>
#define dat P2
#include "lcd.h"
#include "serial.h"
sbit Buzz=P2^1;
sbit DHT11=P2^0;  /* Connect DHT11 output Pin to P2.1 Pin */
int I_RH,D_RH,I_Temp,D_Temp,CheckSum;
extern unsigned char key;

void delay (int);
void display (unsigned char);
void cmd (unsigned char);
void init (void);
void string (char *);
void intro (void);
unsigned char degree[8]={0x0e,0x0a,0x0e,0x00,0x00,0x00,0x00,0x00};
char i=0;

void disp_val(char row, char pos,unsigned char val)
{
  lcd_gotoxy(row, pos);
  lcd_data(val/10 + '0');
  lcd_data(val%10 + '0');
  lcd_gotoxy(1, 15);
}

void delay (int d)
{
 unsigned char i=0;
 for(;d>0;d--)
 {
  for(i=250;i>0;i--);
  for(i=248;i>0;i--);
 }
}

void timer_delay20ms()  /* Timer0 delay function */
{
 //FFFF-B80C+1=47F4=18420*1.085us=20 ms
 TMOD = 0x01;
 TH0 = 0xB8;  /* Load higher 8-bit in TH0 */
 TL0 = 0x0C;  /* Load lower 8-bit in TL0 */
 TR0 = 1;  /* Start timer0 */
 while(TF0 == 0); /* Wait until timer0 flag set */
 TR0 = 0;  /* Stop timer0 */
 TF0 = 0;  /* Clear timer0 flag */
}

void timer_delay30us()  /* Timer0 delay function */
{

 TMOD = 0x01;  /* Timer0 mode1 (16-bit timer mode) */
 TH0 = 0xFF;  /* Load higher 8-bit in TH0 */
 TL0 = 0xF1;  /* Load lower 8-bit in TL0 */
 TR0 = 1;  /* Start timer0 */
 while(TF0 == 0); /* Wait until timer0 flag set */
 TR0 = 0;  /* Stop timer0 */
 TF0 = 0;  /* Clear timer0 flag */
}

void Request()   /* Microcontroller send request */
{
 DHT11 = 0;  /* set to low pin */
 timer_delay20ms(); /* wait for 20ms */
 DHT11 = 1;  /* set to high pin */
}

void Response()   /* Receive response from DHT11 */
{
 while(DHT11==1);
 while(DHT11==0);
 while(DHT11==1);
}

int Receive_data()  /* Receive data */
{
 int q,c=0;
 for (q=0; q<8; q++)
 {
  while(DHT11==0);/* check received bit 0 or 1 */
  timer_delay30us();
  if(DHT11 == 1) /* If high pulse is greater than 30ms */
   c = (c<<1)|(0x01);/* Then its logic HIGH */
  else  /* otherwise its logic LOW */
    c = (c<<1);
  while(DHT11==1);
 }
 return c;
}

void main()
{
 lcd_reset();
  lcd_init();
 lcd_gotoxy_str(0, 0 , "Temperature: "); 
 Buzz=1;
 while(1)
 {
  Request(); /* send start pulse */
  Response(); /* receive response */
  I_RH=Receive_data(); /* store first eight bit in I_RH */
  D_RH=Receive_data(); /* store next eight bit in D_RH */
  I_Temp=Receive_data(); /* store next eight bit in I_Temp */
  D_Temp=Receive_data(); /* store next eight bit in D_Temp */
  disp_val(1, 4,I_Temp);
   delay(5000);
    if(I_Temp>= 32){
   Buzz=0;
						delay(1000);
       lcd_reset();
       lcd_init();
       lcd_gotoxy_str(0,0,"Overheat!!");
   //waiting for command from hyperterminal to stop buzzer
       setup_serial();
   key = __getchar();
     __putchar(key);
      switch(key)
      {
     case 'S':
     case 's':
       Buzz=1; 
      lcd_reset();
  lcd_init();
         printString("\r\nBuzzer stopped\r\n");
       lcd_gotoxy_str(0,0,"Buzzer Stopped");
     delay(1000);
     lcd_reset();
  lcd_init();
      lcd_gotoxy_str(0, 0 , "Temperature: ");
         break;
       }

 }
}
 }