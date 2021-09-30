
#ifndef __IRSENDER_H
#define __IRSENDER_H
#include <IRremoteInt.h>
#include "game.h"
#include "gamelistener.h"

#define IR_SEND_PIN 3

class IRSender : public GameListener
{
private:
    Game* game;

public:
    IRSender();
    void setup(Game* game);
    void onGameEvent(GameEventType gameEventType);
};

#endif