#ifndef __LEDANIMATOR_H
#define __LEDANIMATOR_H

#include "gameeventtype.h"
#include "game.h"


enum AnimationType {
    ANIMATION_HIT,
    ANIMATION_IDLE,
    ANIMATION_FIRE,
    ANIMATION_GAME_OVER
};

class LedAnimator : public GameListener
{
private:
    unsigned long lastAnimationFrameTime;
    unsigned long lastUpdateTime;
    bool isOn;
    Game* game;
    AnimationType animationType;
    
    void handleIdleAnimation();
    void handleFireAnimation();
    void handleHitAnimation();
    void handleGameOverAnimation();

public:
    LedAnimator();
    void setup(Game* game);
    void onGameEvent(GameEventType gameEventType);
};

#endif