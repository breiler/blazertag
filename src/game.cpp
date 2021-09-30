#include <Arduino.h>
#include "game.h"

Game::Game()
{
    isFiring = false;
    for (uint8_t i = 0; i < NUMBER_OF_LISTENERS; i++)
    {
        listeners[i] = 0;
    }
}

void Game::handleFire()
{
    if (isFiring)
    {
        if (health == 0)
        {
            lastFireTime = millis() - maxCoolDown;
            notifyEvent(GameEventType::FIRING_SHOT_NO_HEALTH);
        }
        else if (lastFireTime + maxCoolDown < millis())
        {
            if (ammo > 0)
            {
                ammo--;
                lastFireTime = millis();
                notifyEvent(GameEventType::FIRING_SHOT);
                Serial.print(F("Game::update: Firing, remaining ammo "));
                Serial.println(ammo);
            }
            else
            {
                lastFireTime = millis() - maxCoolDown;
                notifyEvent(GameEventType::FIRING_SHOT_NO_AMMO);
                Serial.println(F("Game::update: No more ammo..."));
            }
        }
        else
        {
            notifyEvent(GameEventType::FIRING_SHOT_COOLING_DOWN);
        }
    }
}

void Game::handleHit()
{
    if (health > 0)
    {
        health--;
        Serial.print(F("Game::onShot: Players health "));
        Serial.println(health);
        if (health == 0)
        {
            notifyEvent(GameEventType::HIT_KILLED);
        }
        else
        {
            notifyEvent(GameEventType::HIT);
        }
    }
    else
    {
        Serial.println(F("Game::onShot: Player dead"));
        notifyEvent(GameEventType::HIT_DEAD);
    }
}

void Game::notifyEvent(GameEventType eventType)
{
    for (int i = 0; i < NUMBER_OF_LISTENERS; i++)
    {
        if (listeners[i])
        {
            listeners[i]->onGameEvent(eventType);
        }
    }
}

void Game::update()
{
    handleFire();
    notifyEvent(GameEventType::GAME_TICK);
}

void Game::setup(uint8_t teamId, uint16_t maxHealth, uint16_t maxAmmo, uint16_t maxCoolDown)
{
    // Try to create randomness and generate a player id
    randomSeed(analogRead(A0));
    playerId = random();
    teamId = teamId;

    Serial.print(F("Game::setup: Starting as player #"));
    Serial.print(playerId);
    Serial.print(F(" on team "));
    Serial.println(teamId);

    health = maxHealth;
    this->maxHealth = maxHealth;

    ammo = maxAmmo;
    this->maxAmmo = maxAmmo;

    this->maxCoolDown = maxCoolDown;
    lastFireTime = millis() - maxCoolDown;

    notifyEvent(GameEventType::GAME_INITALIZED);
}

void Game::onReceivedShot(uint8_t teamId, uint8_t playerId)
{
    bool isFreeForAll = teamId == 0 && this->teamId == 0;
    bool isShotFromOtherPlayer = playerId != this->playerId;

    if ((isFreeForAll || teamId != this->teamId) && isShotFromOtherPlayer)
    {
        Serial.print(F("Game::onShot: Received a shot from player "));
        Serial.print(playerId);
        Serial.print(F(" on team "));
        Serial.println(teamId);
        handleHit();
    }
}

void Game::onFire()
{
    isFiring = true;
}

void Game::onStopFire()
{
    isFiring = false;
}

uint8_t Game::getPlayerId()
{
    return playerId;
}

uint16_t Game::getMaxHealth()
{
    return maxHealth;
}

uint16_t Game::getCurrentCoolDown()
{
    if (lastFireTime + maxCoolDown < millis())
    {
        return 0;
    }
    return lastFireTime + maxCoolDown - millis();
}

uint16_t Game::getMaxCoolDown()
{
    return maxCoolDown;
}

void Game::addListener(GameListener *listener)
{
    for (int i = 0; i < NUMBER_OF_LISTENERS; i++)
    {
        if (listeners[i] == 0)
        {
            listeners[i] = listener;
            return;
        }
    }
}

uint16_t Game::getCurrentHealth()
{
    return health;
}

uint16_t Game::getCurrentAmmo()
{
    return ammo;
}

uint16_t Game::getMaxAmmo()
{
    return maxAmmo;
}

uint8_t Game::getTeamId()
{
    return teamId;
}