#include "sprites.h"
#include "lcd.h"

static pixelArray carR = {
  0b00000,
  0b00000,
  0b11010,
  0b11111,
  0b11010,
  0b00000,
  0b00000,
  0b00000,
};

static pixelArray boom1 = {
  0b00000,
  0b00100,
  0b01110,
  0b11011,
  0b01110,
  0b00100,
  0b00000,
  0b00000,
};

static pixelArray boom2 = {
  0b00100,
  0b01110,
  0b11011,
  0b10001,
  0b11011,
  0b01110,
  0b00100,
  0b00000,
};

static pixelArray boom3 = {
  0b01110,
  0b11011,
  0b10001,
  0b00000,
  0b10001,
  0b11011,
  0b01110,
  0b00000,
};

static pixelArray shot = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};

static pixelArray carObs = {
  0b00000,
  0b00000,
  0b10010,
  0b11111,
  0b11111,
  0b10010,
  0b00000,
  0b00000,
};

static pixelArray carL = {
  0b00000,
  0b00000,
  0b01011,
  0b11111,
  0b01011,
  0b00000,
  0b00000,
  0b00000,
};

static pixelArray powerUp1 = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000,
};

static pixelArray hole = {
  0b01110,
  0b11011,
  0b10001,
  0b10001,
  0b10001,
  0b11011,
  0b01110,
  0b00000,
};

static pixelArray powerUp2 = {
  0b00010,
  0b00110,
  0b01100,
  0b11111,
  0b00110,
  0b01100,
  0b01000,
  0b00000,
};

static pixelArray holeShot = {
  0b01110,
  0b11011,
  0b10001,
  0b11111,
  0b10001,
  0b11011,
  0b01110,
  0b00000,
};

static pixelArray *spritesShared[] = {&carR, &boom1, &boom2, &boom3, &shot, 0},
                      *spritesSP[] = {&carObs, &hole, &holeShot, 0},
                      *spritesMP[] = {&carL, &powerUp1, &powerUp2, 0};

static unsigned char dark = 1, playMode = 0;

void LoadSprites(void) {
    unsigned char i;
    for (i = 0; spritesShared[i]; i++) {
        if (dark) LCD_LoadChar(i, *spritesShared[i]);
    }
    pixelArray **sprites2 = (!playMode? spritesSP : spritesMP);
    for (; **sprites2; i++) {
        LCD_LoadChar(i, **sprites2++);
    }
    emptyChar = ' ';
    dark = 0;
}

void LoadInverse(void) {
    unsigned char i;
    for (i = 0; spritesShared[i]; i++) {
        if (!dark) LCD_LoadCharInverse(i, *spritesShared[i]);
    }
    pixelArray **sprites2 = (!playMode? spritesSP : spritesMP);
    for (; **sprites2; i++) {
        LCD_LoadCharInverse(i, **sprites2++);
    }
    emptyChar = 255;
    dark = 1;
}

void SetPlayMode(unsigned char mode) {
    if (playMode != mode) {
        playMode = mode;
        if (dark) LoadInverse();
        else LoadSprites();
    }
}
