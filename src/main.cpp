#include <Arduino.h>
#include "game.h"
#include "input.h"
#include "listeners/ledanimator.h"
#include "listeners/oled.h"
#include "listeners/irsender.h"

// Game attributes
#define TEAM_ID 0
#define MAX_LIFE 10
#define MAX_AMMO 999
#define COOL_DOWN 300 //ms

// IO
#define TRIGGER_PIN 2
#define IR_RECEIVER_PIN 12

Input input;
Game game;
LedAnimator ledAnimator;
Oled oled;
IRSender sender;

void setup()
{
    // Initialize the serial output
    Serial.begin(9600);
    delay(2000);
    while (!Serial)
        ; //delay for Leonardo

    // Initialize the game engine
    game.setup(TEAM_ID, MAX_LIFE, MAX_AMMO, COOL_DOWN);
    input.setup(IR_RECEIVER_PIN, TRIGGER_PIN);
    input.addListener(&game);

    // Add light visualizations
    ledAnimator.setup(&game);
    game.addListener(&ledAnimator);

    // Add Oled
    oled.setup(&game);
    game.addListener(&oled);

    sender.setup(&game);
    game.addListener(&sender);
}

void loop()
{
    input.update();
    game.update();
}
