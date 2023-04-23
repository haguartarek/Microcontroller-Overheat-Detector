
#ifndef _LCD_
#define _LCD_

 
void lcd_cmd(char cmd);

void lcd_data (char dat)  ;


void lcd_reset();
	
void lcd_string (char *dat);

void lcd_gotoxy(char row, char pos);

void lcd_gotoxy_str(char row, char pos, char *str);

void lcd_clear();

void lcd_init ();
	


#endif