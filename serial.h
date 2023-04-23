
#ifndef _SERIAL_
#define _SERIAL_



void setup_serial();
void __putchar(unsigned char ch);
unsigned char __getchar();
void printString(const char *s);
unsigned long  GetValue( unsigned char max_len);

void shift_disp_left();
void shift_disp_right();
#endif