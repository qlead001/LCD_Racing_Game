#include <string.h>
#include "lcd_basic.h"
#include "lcd.h"

unsigned char screenBuffer[32], updateBuffer[32];
unsigned char emptyChar = ' ';

void LCD_ClearScreen(void) {
    LCD_WriteCommand(0x01);
    memset(screenBuffer, emptyChar, 32);
}

void LCD_DisplayString(unsigned char column, const char* string) {
    LCD_ClearScreen();
    LCD_Cursor(column);
    while(*string) {
        LCD_WriteData(*string++);
    }
}

void LCD_DisplayCenter(unsigned char row, const char* string) {
    unsigned char pad = (16-strlen(string))/2;
    if (pad >= 0) { // Fails if string is longer than 16
        LCD_Cursor(15*row+pad);
        while(*string) {
            LCD_WriteData(*string++);
        }
    }
}

void LCD_Cursor(unsigned char column) {
   if ( column < 16 ) { // 16x1 LCD: column < 8
						// 16x2 LCD: column < 16
      LCD_WriteCommand(0x80 + column);
   } else {
      LCD_WriteCommand(0xB8 + column - 8);	// 16x1 LCD: column
											// 16x2 LCD: column - 8
   }
}

void LCD_LoadChar(unsigned char pos, pixelArray customChar) {
    if (pos < 8) { // Only supports 8 custom characters
        LCD_WriteCommand(0x40 + 8*pos); // Points to CGRAM address
        for (unsigned char i = 0; i < 8; i++) {
            LCD_WriteData(customChar[i]);
        }
    }
}

void LCD_LoadCharInverse(unsigned char pos, pixelArray customChar) {
    if (pos < 8) { // Only supports 8 custom characters
        LCD_WriteCommand(0x40 + 8*pos); // Points to CGRAM address
        for (unsigned char i = 0; i < 8; i++) {
            LCD_WriteData(customChar[i] ^ 0x1F); // ^ 0x1F inverts bits 0 to 5
        }
    }
}

void Screen_AddCh(unsigned char pos, unsigned char ch) { // maybe change to macro
    updateBuffer[pos] = ch;
}

void Screen_AddStr(unsigned char pos, const char *string) {
    while (*string) {
        updateBuffer[pos++] = *string++;
    }
}

void Screen_CenterStr(unsigned char row, const char *string) {
    unsigned char pad = (16-strlen(string))/2;
    if (pad >= 0) { // Fails if string is longer than 16
        unsigned char pos = 15*row+pad;
        while(*string) {
            updateBuffer[pos++] = *string++;
        }
    }
}

void Screen_Fill(unsigned char ch) { // maybe change to macro
    memset(updateBuffer, ch, 32);
}

void Screen_Clear(void) {
    memset(updateBuffer, emptyChar, 32);
}

void Screen_Refresh(void) {
    LCD_Cursor(0);
    unsigned char prev = 0;
    for (unsigned char i = 0; i < 32; i++) {
        unsigned char newChar = updateBuffer[i];
        if (newChar != screenBuffer[i]) {
            /*if (i && (i != prev+1 || i == 16))*/ LCD_Cursor(i);
            LCD_WriteData(newChar);
            screenBuffer[i] = newChar;
            prev = i;
        }
    }
}
