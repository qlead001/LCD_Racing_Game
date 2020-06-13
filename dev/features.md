# Features

## Implemented

* Interactive Menu
  * With animation
* Single player mode
  * Two types of obstacles
    * Holes cannot be shot
    * Parked cars can be shot
  * Player fires lasers
    * Laser has a brief cooldown period
  * Player and parked cars explode
  * Animated explosions
  * Speeds up over time
  * Obstacles become more frequent over time
  * Score tracks distance traveled
* Pause menu
* Game over screen with score
  * Pressing pause quits to menu
  * Shooting quick starts a new round
* Screen abstraction for LCD
  * Reduces amount of writing to LCD
* Sprite file for loading custom sprites
* Dev tools
  * Animation test
    * `make program-anim`
    * Press A0 to shoot
  * Char test
    * `make program-char`
    * Press A0 to cycle through pages
    * PORTB displays page number

## Unimplemented

* Two player battle mode
  * Uses different sprites
  * Powerups
* Interactive pause menu
* Dark Mode
* Life system
* 4-bit LCD interfacing
  * Makefile to link with either LCD interface
* A readme

## Unintended

* Player may be able to phase through
obstacles sometimes
* Commented out code in source/lcd.c
breaks screen display code
* GCC gives lots of pretty warnings about
signedness of pointers
* A test file that does nothing
