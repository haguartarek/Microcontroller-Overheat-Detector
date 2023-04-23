#define LCD     	  P1
#define ENABLE      0x04
#define REG_SL      0x02

#include <reg51.h>
#include "lcd.h"

void delay()
{
   int i,j;
   for(i=0;i<=500;i++);
			for(j=0;j<=500;j++);
}
 

void lcd_reset()                   //Reset sequence as stated in the LCD datasheet
{
		LCD = 0xFF;
		delay();
		LCD = 0x30|ENABLE;
		LCD = 0x30;
		delay();
		LCD = 0x30|ENABLE;
		LCD = 0x30;
		delay();
		LCD = 0x30|ENABLE;
		LCD = 0x30;
		delay();
		LCD = 0x20|ENABLE;
		LCD = 0x20;
		delay();
}

void lcd_cmd(char cmd)               //Subroutine to send command
{ 
	LCD = (cmd & 0xF0)| ENABLE;     //Sending higher nibble by shifting with Pulsing Enable high
	delay();
	LCD = (cmd & 0xF0);
	delay();
	LCD = ((cmd & 0x0F) << 4) | ENABLE;          //Sending lower nibble Enable low
	LCD = ((cmd & 0x0F) << 4);
}


void lcd_data (char dat)          //Subroutine to send data
{ 
	LCD = ((dat & 0xF0) | ENABLE | REG_SL); //Sending data by making RS=1
	delay();
	LCD = ((dat & 0xF0) | REG_SL);
	delay();
	LCD = (((dat & 0x0F) << 4) | ENABLE | REG_SL);
	delay();
	LCD = (((dat & 0x0F) << 4) | REG_SL);
	delay();
}
	
	
void lcd_string (char *dat)          //Subroutine to send data
{ 
    while(*dat != '\0' )
	{
	LCD = ((*dat & 0xF0) | ENABLE | REG_SL); //Sending data by making RS=1
	delay();
	LCD = ((*dat & 0xF0) | REG_SL);
	delay();
	LCD = (((*dat & 0x0F) << 4) | ENABLE | REG_SL);
	delay();
	LCD = (((*dat & 0x0F) << 4) | REG_SL);
	delay();
	dat++;
	}
}



void lcd_gotoxy(char row, char pos)
{
	
	if(row ==0 && pos <16 )
		lcd_cmd((pos & 0x0F) | 0x80);
	else if( row ==1 && pos < 16)
		lcd_cmd((pos & 0x0F) | 0xC0);
}

void lcd_gotoxy_str(char row, char pos, char *str)
{
	lcd_gotoxy(row, pos);
	lcd_string(str);
}

void lcd_clear()
{
	lcd_cmd(0x01);				// clear display
	lcd_cmd(0x02);				//Clear screen and return home
	lcd_cmd(0x80);				//cursor at home position
}
 

void lcd_init ()
{
	lcd_cmd(0x28);       // 4-bit mode - 2 line - 5x7 font. 
 	lcd_cmd(0x0F);       // Display cursor with blink.
	lcd_cmd(0x06);       // Automatic Increment - No Display shift.

  lcd_clear();
}
	
void shift_disp_left()
{
	lcd_cmd(0x18);
}


void shift_disp_right()
{
	lcd_cmd(0x1C);
}

