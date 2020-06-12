/*	Author: qlead001@ucr.edu
 *	Description: Testing animations on LCD
 *  Build: make program-anim
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

unsigned char cnt = 0, old0 = 0, shot = 0, cntPeriod = 0, dark = 0;

void displayAnimation(void) {
    unsigned char A0 = (~PINA)&0x01;
    if (A0 != old0) {
        old0 = A0;
        if (A0) {
            shot = 1;
            Screen_Clear();
        }
    }
    cntPeriod++;
    if (cntPeriod > 50) {
        if (dark) LoadSprites();
        else LoadInverse();
        dark = !dark;
        cntPeriod = 0;
        Screen_Clear();
    }

    if (cnt<6)
        Screen_AddCh(0, CAR1);
    else if (cnt>9)
        Screen_AddCh(0, emptyChar);
    else
        Screen_AddCh(0, CAR1+cnt-6);
    cnt = (cnt+1)%13;

    if (shot == 16) {
        shot = 0;
        Screen_AddCh(14, emptyChar);
    } else if (shot > 0) {
        if (shot > 2) {
            Screen_AddCh(shot-2, emptyChar);
        }
        Screen_AddCh(shot, SHOT);
        shot++;
    } else {
        Screen_AddCh(15, emptyChar);
    }
    Screen_Refresh();
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    LCD_init();
    LoadSprites();
    Screen_Clear();
    Screen_Refresh();
    TimerSet(200);
    TimerOn();
    while (1) {
        displayAnimation();
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}

/*void displayAnimation(void) {
    LCD_Cursor(0);
    if (cnt<6)
        LCD_WriteData(CAR_RIGHT);
    else if (cnt>9)
        LCD_WriteData(emptyChar);
    else
        LCD_WriteData(CAR_RIGHT+cnt-6);
    cnt = (cnt+1)%13;
    unsigned char A0 = (~PINA)&0x01;
    if (A0 != old0) {
        old0 = A0;
        if (A0) {
            shot = 1;
            LCD_ClearScreen();
        }
    }
    if (shot == 16) {
        shot = 0;
        LCD_Cursor(14);
        LCD_WriteData(emptyChar);
    } else if (shot > 0) {
        if (shot > 2) {
            LCD_Cursor(shot-2);
            LCD_WriteData(emptyChar);
        }
        LCD_Cursor(shot);
        LCD_WriteData((shot%2)?SHOT1:SHOT2);
        shot++;
    } else {
        LCD_Cursor(15);
        LCD_WriteData(emptyChar);
    }
}*/
