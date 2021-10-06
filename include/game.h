#ifndef __GAME_H
#define __GAME_H

#include <Arduino.h>
#include "inputlistener.h"
#include "gamelistener.h"
#include "gameeventtype.h"
#define NUMBER_OF_LISTENERS 10

class Game : public InputListener
{
private:
    GameListener* listeners[NUMBER_OF_LISTENERS];

    bool isFiring;
    unsigned long lastFireTime;

    uint8_t playerId;
    uint8_t teamId;

    uint16_t health;
    uint16_t ammo;

    uint16_t maxHealth;
    uint16_t maxAmmo;
    uint16_t maxCoolDown;

    void handleFire();
    void handleHit();
    void onReceivedShot(uint8_t teamId, uint8_t playerId);
    void onFire();
    void onStopFire();
    void notifyEvent(GameEventType eventType);

public:
    Game();
    void setup(uint8_t playerId, uint8_t teamId, uint16_t maxHealth, uint16_t maxAmmo, uint16_t maxCoolDown);
    void update();
    void addListener(GameListener *listener);

    uint8_t getTeamId();
    uint8_t getPlayerId();
    uint16_t getCurrentHealth();
    uint16_t getMaxHealth();
    uint16_t getCurrentAmmo();
    uint16_t getMaxAmmo();
    uint16_t getCurrentCoolDown();
    uint16_t getMaxCoolDown();
};

#endif