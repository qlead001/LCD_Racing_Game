#ifndef __lcd_basic_h__
#define __lcd_basic_h__

void LCD_init();
void LCD_WriteCommand(unsigned char Command);
void LCD_WriteData(unsigned char Data);
//void delay_ms(int miliSec);
#endif
