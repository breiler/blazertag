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
                lastFireTime = millis();
                notifyEvent(GameEventType::FIRING_SHOT);
                ammo--;
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

void Game::setup(uint8_t teamId, uint8_t maxHealth, uint8_t maxAmmo, uint16_t maxCoolDown)
{
    // Try to create randomness and generate a player id
    randomSeed(analogRead(A0));
    playerId = random() % 1024;
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

void Game::onReceivedShot(uint32_t code)
{
    if (code == 88)
    {
        Serial.print(F("Game::onShot: Received a shot from "));
        Serial.println(code);
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

uint8_t Game::getMaxHealth()
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

uint8_t Game::getCurrentHealth()
{
    return health;
}