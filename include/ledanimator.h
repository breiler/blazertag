#ifndef __LEDANIMATOR_H
#define __LEDANIMATOR_H

#include "game.h"
#include "gamelistener.h"

enum AnimationType {
    ANIMATION_HIT,
    ANIMATION_IDLE,
    ANIMATION_FIRE
};

class LedAnimator : public GameListener
{
private:
    Game *game;
    unsigned long lastAnimationFrameTime;
    unsigned long lastUpdateTime;
    bool isOn;
    AnimationType animationType;
    
    void handleIdleAnimation();
    void handleFireAnimation();
    void handleHitAnimation();

public:
    LedAnimator();
    void setup(Game *game);
    void onGameEvent(GameEventType event);
};

#endif