#ifndef __OLED_H
#define __OLED_H

#include <Arduino.h>
#include <ss_oled.h>
#include "game.h"

// Change these if you're using a different OLED display
#define MY_OLED OLED_128x64
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Use -1 for the Wire library default pins
// or specify the pin numbers to use with the Wire library or bit banging on any GPIO pins
// These are the pin numbers for the M5Stack Atom default I2C
#define SDA_PIN A4
#define SCL_PIN A5
// Set this to -1 to disable or the GPIO pin number connected to the reset
// line of your display if it requires an external reset
#define RESET_PIN -1
// let ss_oled figure out the display address
#define OLED_ADDR 0x3C
// don't rotate the display
#define FLIP180 0
// don't invert the display
#define INVERT 0
// Bit-Bang the I2C bus
#define USE_HW_I2C 1

static uint8_t *ucBackBuffer = NULL;

enum DisplayState {
    DISPLAY_STATUS,
    DISPLAY_GAME_OVER
};

class Oled : public GameListener
{
private:
    SSOLED ssoled;
    DisplayState displayState;
    unsigned long lastUpdateTime;
    Game* game;
    void displayStatus(bool inverseHealth, bool inverseAmmo);
    void displayGameOver(bool inverse);

public:
    Oled();
    void setup(Game* game);
    void onGameEvent(GameEventType gameEventType);
};

#endif