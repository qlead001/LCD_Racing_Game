/*	Author: qlead001@ucr.edu
 *  Partner(s) Name: NA
 *	Lab Section: 026
 *	Assignment: Final Option #2
 *	Exercise Description: Racing game on LCD
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <avr/io.h>
#include "timer.h"
#include "scheduler.h"
#include "lcd.h"
#include "sprites.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#define CAR_MASK 1
#define HOLE_MASK 2
#define OBS_MASK 3
#define LASER_MASK 4
#define BOOM_MASK 24
#define BOOM_SHIFT 3

// -----Shared Variables-----
unsigned char pause = 0, left = 0, gameover = 0, shoots = 0, explode = 0,
              right = 0, update = 1, playerPos = 0, playerRow = 0, menu = 1;
unsigned short score = 0;
unsigned char gameBoard[32] = {0};
// --------------------------

enum Button_States { input };

unsigned char old0 = 0, old1 = 0, old2 = 0, old3 = 0, old4 = 0;

int ButtonTick(int state) {
    unsigned char in = (~PINA);
    unsigned char A0 = (in&0x01), A1 = (in&0x02), A2 = (in&0x04),
                  A3 = (in&0x08), A4 = (in&0x10);
    switch (state) {
        case input:
            if (A0 != old0) {
                old0 = A0;
                if (A0 && !menu) pause = pause?0:1;
            }
            if (A1 != old1) {
                old1 = A1;
                if (A1 && !explode) {
                    playerRow = playerRow?0:1;
                    update = 1;
                }
            }
            if (A2 != old2) {
                old2 = A2;
                if (A2) shoots = 1;
            }
            if (A3 != old3) {
                old3 = A3;
                if (A3) right = 1;
            }
            if (A4 != old4) {
                old4 = A4;
                if (A4) left = 1;
            }
            break;
        default: state = input; break;
    }
    return state;
}

enum Game_States { gameMenu, gamePause, gamePlay };

const unsigned char maxPeriod = 6, startDiff = 50, laserTimeout = 5;
unsigned char countPeriod, countAnim = 0, periods, cycles, difficulty, lastObs, start = 1, laserTime = 0;

void clearGame(void) {
    memset(gameBoard, 0, 32);
}

void laserMove(unsigned char pos) {
    if (gameBoard[pos-1]&LASER_MASK) {
        if (gameBoard[pos-1]&CAR_MASK) {
            gameBoard[pos-1] = BOOM_MASK;
        } else if (gameBoard[pos]&CAR_MASK) {
            gameBoard[pos] = BOOM_MASK;
        } else {
            gameBoard[pos] |= LASER_MASK;
        }
    } else gameBoard[pos] &= ~LASER_MASK;
}

unsigned char replaceMask(unsigned char replace, unsigned char copy, unsigned char mask) {
    replace &= ~mask;
    copy &= mask;
    return (replace | copy);
}

int GameTick(int state) {
    switch (state) {
        case gameMenu:
            if (shoots) {
                if (start) srand(time(NULL));
                score = 0;
                playerPos = 0;
                gameover = start = menu = 0;
                periods = maxPeriod;
                difficulty = startDiff;
                lastObs = 5;
                countPeriod = cycles = 0;
                clearGame();
                playerRow = left = right = shoots = 0;
                state = gamePlay;
            }
            break;
        case gamePause:
            if (!pause || shoots) {
                if (!pause && gameover) state = gameMenu;
                else state = gamePlay;
                pause = 0;
                if (gameover) {
                    score = 0;
                    playerPos = 0;
                    gameover = 0;
                    periods = maxPeriod;
                    difficulty = startDiff;
                    countPeriod = cycles = 0;
                    lastObs = 5;
                    clearGame();
                }
                left = right = shoots = 0;
            }
            break;
        case gamePlay:
            if (pause) {
                state = gamePause;
                update = 1;
            }else{
                if (left) {
                    if (playerPos > 0 && !explode)playerPos--;
                    left = right = 0;
                } else if (right) {
                    if (playerPos < 15 && !explode)playerPos++;
                    left = right = 0;
                }
                if (gameBoard[playerRow*16+playerPos]&CAR_MASK) {
                    update = 1;
                    gameBoard[playerRow*16+playerPos] &= ~CAR_MASK;
                    explode = 1;
                }
                if (!explode&&(gameBoard[playerRow*16+playerPos]&HOLE_MASK)) {
                    update = gameover = pause = 1;
                    state = gamePause;
                } else if (countPeriod++ == periods) {
                    countPeriod = 0;
                    update = 1;
                    for (unsigned char i = 0; i < 15; i++) {
                        gameBoard[i] = replaceMask(gameBoard[i], gameBoard[i+1], OBS_MASK+BOOM_MASK);
                        gameBoard[16+i] = replaceMask(gameBoard[16+i], gameBoard[i+17], OBS_MASK+BOOM_MASK);
                    }
                    gameBoard[15] &= ~(OBS_MASK+BOOM_MASK); gameBoard[31] &= ~(OBS_MASK+BOOM_MASK);
                    if (!(rand() % (difficulty/lastObs))) {
                        gameBoard[16*(rand()%2)+15] |= (rand()%2)?CAR_MASK:HOLE_MASK;
                        lastObs = 1;
                    } else lastObs++;
                    if (cycles++ == 10) {
                        cycles = 0;
                        if (periods > 1) periods--;
                        else if (difficulty > 8) difficulty -= 5;
                    }
                    score += 1*(!explode);//+maxPeriod-periods+((startDiff-difficulty)/5);
                }
                if ((countAnim = !countAnim)) {
                    update = 1;
                    laserTime--;
                    for (unsigned char i = 15; i > 0; i--) {
                        if (gameBoard[i]&BOOM_MASK) gameBoard[i] &= ((gameBoard[i]>>BOOM_SHIFT)-1)<<BOOM_SHIFT;
                        laserMove(i);
                        if (gameBoard[16+i]&BOOM_MASK) gameBoard[16+i] &= ((gameBoard[16+i]>>BOOM_SHIFT)-1)<<BOOM_SHIFT;
                        laserMove(16+i);
                    }
                    if (!laserTime && shoots && playerPos < 15) {
                        gameBoard[16*playerRow+playerPos+1] |= LASER_MASK;
                        laserTime = laserTimeout;
                        shoots = 0;
                    }
                    if (explode>0) {
                        if (explode++>=3) {
                            gameover = pause = 1;
                            state = gamePause;
                            explode = 0;
                        }
                    }
                }
            }
            break;
        default: state = gameMenu; break;
    }
    return state;
}

enum LCD_States { output };

void scoreString(unsigned short score, unsigned char* buf) {
    strcpy(buf, "Score: ");
    unsigned char suppress = 1;
    unsigned char i = strlen(buf);
    for (unsigned short div = 10000; div > 1; div /= 10) {
        unsigned char num = (unsigned char)(score/div);
        score = score % div;
        if (!suppress || num > 0) {
            suppress = 0;
            buf[i++] = num+'0';
        }
    }
    buf[i] = (unsigned char)(score)+'0';
    buf[i+1] = '\0';
}

unsigned char Render(unsigned char num) {
    unsigned char c = emptyChar;
    if (num&BOOM_MASK) {
        num = (num&BOOM_MASK)>>BOOM_SHIFT;
        if (num==3) c = BOOM1;
        else if (num==2) c = BOOM2;
        else if (num==1) c = BOOM3;
    } else if (num&CAR_MASK) c = CAR2;
    else if (num&HOLE_MASK) {
        if (num&LASER_MASK) c = HOLE_SHOT;
        else c = HOLE;
    } else if (num&LASER_MASK) c = SHOT;
    else c = emptyChar;
    return c;
}

int LCDTick(int state) {
    switch (state) {
        case output: state = output; break;
        default: state = output; break;
    }
    switch (state) {
        case output:
            if (update) {
                update = 0;
                if (menu) {
                    Screen_Clear();
                    Screen_CenterStr(0, "1 Player");
                    Screen_CenterStr(1, "2 Player");
                    Screen_AddCh(playerRow*16, CAR1);
                }else if (gameover && pause) {
                    unsigned char buf[13];
                    Screen_Clear();
                    Screen_CenterStr(0, "Game Over!");
                    scoreString(score, buf);
                    Screen_CenterStr(1, buf);
                } else if (pause) {
                    Screen_CenterStr(0, "Paused");
                }else{
                    for (unsigned char i = 0; i < 32; i++) {
                        Screen_AddCh(i, Render(gameBoard[i]));
                    }
                    if (!(gameBoard[playerRow*16+playerPos]&BOOM_MASK)) {
                        Screen_AddCh(playerRow*16+playerPos, CAR1);
                        if (explode) {
                            if (explode==1) Screen_AddCh(playerRow*16+playerPos, BOOM1);
                            else if (explode==2) Screen_AddCh(playerRow*16+playerPos, BOOM2);
                            else if (explode==3) Screen_AddCh(playerRow*16+playerPos, BOOM3);
                        }
                    }
                }
                Screen_Refresh();
            }
            break;
    }
    return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    static task task1, task2, task3;
    task *tasks[] = { &task1, &task2, &task3 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = input;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &ButtonTick;

    task2.state = gameMenu;
    task2.period = 100;
    task2.elapsedTime = task2.period;
    task2.TickFct = &GameTick;

    task3.state = output;
    task3.period = 100;
    task3.elapsedTime = task3.period;
    task3.TickFct = &LCDTick;

    unsigned short i;
    unsigned long GCD = tasks[0]->period;
    for (i = 1; i < numTasks; i++) {
        GCD = findGCD(GCD, tasks[i]->period);
    }
    TimerSet(GCD);
    TimerOn();
    LCD_init();
    LoadSprites();

    while (1) {
        for (i = 0; i < numTasks; i++) {
            if (tasks[i]->elapsedTime >= tasks[i]->period) {
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += GCD;
        }
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
