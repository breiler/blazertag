#ifndef __GAMELISTENER_H
#define __GAMELISTENER_H

#include "gameeventtype.h"

class GameListener
{
public:
    virtual void onGameEvent(GameEventType gameEventType) = 0;
};

#endif