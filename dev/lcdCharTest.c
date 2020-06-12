/*	Author: qlead001@ucr.edu
 *	Description: Testing what characters the lcd has
 *  Build: make program-chars
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "timer.h"
#include "lcd.h"
#include "sprites.h"

unsigned char pos = 0, prev = 1, old0 = 0;

void displayChars(void) {
    if (pos != prev) {
        prev = pos;
        PORTB = pos;
        for (unsigned char i = 0; i < 16; i++) {
            LCD_WriteData(i+pos*32);
        }
        LCD_Cursor(16);
        for (unsigned char i = 16; i < 32; i++) {
            LCD_WriteData(i+pos*32);
        }
        LCD_Cursor(0);
    }
    unsigned char A0 = (~PINA)&0x01;
    if (A0 != old0) {
        old0 = A0;
        if (A0) {
            if (pos < 7) pos++;
            else pos = 0;
        }
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    LCD_init();
    LoadSprites();
    LCD_Cursor(0);
    TimerSet(100);
    TimerOn();
    while (1) {
        displayChars();
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
