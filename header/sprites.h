#ifndef __sprites_h__
#define __sprites_h__

enum Sprites {
    CAR1 = 0,
    BOOM1 = 1,
    BOOM2 = 2,
    BOOM3 = 3,
    SHOT = 4,
    CAR2 = 5,
    POWERUP1 = 6,
    HOLE = 6,
    POWERUP2 = 7,
    HOLE_SHOT = 7,
};

void LoadSprites(void);
void LoadInverse(void);
void SetPlayMode(unsigned char mode);
#endif
