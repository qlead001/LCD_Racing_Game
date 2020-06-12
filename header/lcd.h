#ifndef __lcd_h__
#define __lcd_h__

#include "lcd_basic.h"

typedef unsigned char pixelArray[8];

extern unsigned char screenBuffer[32], updateBuffer[32];
extern unsigned char emptyChar;

void LCD_ClearScreen(void);
void LCD_Cursor(unsigned char column);
void LCD_DisplayString(unsigned char column, const char *string);
void LCD_DisplayCenter(unsigned char row, const char *string);
void LCD_LoadChar(unsigned char pos, pixelArray customChar);
void LCD_LoadCharInverse(unsigned char pos, pixelArray customChar);

void Screen_AddCh(unsigned char pos, unsigned char ch);
void Screen_AddStr(unsigned char pos, const char *string);
void Screen_CenterStr(unsigned char row, const char *string);
void Screen_Fill(unsigned char ch);
void Screen_Clear(void);
void Screen_Refresh(void);
#endif
