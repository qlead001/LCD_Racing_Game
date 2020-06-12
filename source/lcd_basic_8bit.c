#define F_CPU 8000000UL // 8MHz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd_basic.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))
          
/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_init(void) {
    //wait for 100 ms.
	_delay_ms(100);
	LCD_WriteCommand(0x38); // 16x2 8-bit mode
	LCD_WriteCommand(0x06); // Auto-Increment cursor
	LCD_WriteCommand(0x0c); // Display on without cursor
	LCD_WriteCommand(0x01); // Clear display
	_delay_ms(10);
    LCD_WriteCommand(0x80); // Send cursor home
}

void LCD_WriteCommand(unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS); // RS=0 Command register
   DATA_BUS = Command;
   SET_BIT(CONTROL_BUS,E); // Pulse Enable
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   _delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS); // RS=1 Data register
   DATA_BUS = Data;
   SET_BIT(CONTROL_BUS,E); // Pulse Enable
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   _delay_ms(1);
}

/*void delay_ms(int miliSec) { // 8 Mhz crystal
    int i,j;
    for(i=0;i<miliSec;i++) {
        for(j=0;j<775;j++) {
            asm("nop");
        }
    }
}*/
