#ifndef __GAME_H
#define __GAME_H

#include <LinkedList.h>
#include "inputlistener.h"
#include "gamelistener.h"

class Game : public InputListener
{
private:
    LinkedList<GameListener *> listeners;

    bool isFiring;
    unsigned long lastFireTime;

    unsigned int playerId;
    unsigned int teamId;

    unsigned int health;
    unsigned int ammo;

    unsigned int maxHealth;
    unsigned int maxAmmo;
    unsigned long maxCoolDown;

    void handleFire();
    void handleHit();
    void onReceivedShot(uint32_t code);
    void onFire();
    void onStopFire();
    void notifyEvent(GameEventType eventType);

public:
    Game();
    void setup(unsigned int teamId, unsigned int maxHealth, unsigned int maxAmmo, unsigned int maxCoolDown);
    void update();
    void addListener(GameListener *listener);

    unsigned int getPlayerId();
    unsigned int getCurrentHealth();
    unsigned int getMaxHealth();
    unsigned int getCurrentAmmo();
    unsigned int getMaxAmmo();
    unsigned int getCurrentCoolDown();
    unsigned int getMaxCoolDown();
};

#endif