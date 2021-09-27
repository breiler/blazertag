#include <Arduino.h>
#include "game.h"

Game::Game()
{
    isFiring = false;
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
                Serial.print("Game::update: Firing, remaining ammo ");
                Serial.println(ammo);
            }
            else
            {
                lastFireTime = millis() - maxCoolDown;
                notifyEvent(GameEventType::FIRING_SHOT_NO_AMMO);
                Serial.println("Game::update: No more ammo...");
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
        Serial.print("Game::onShot: Players health ");
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
        Serial.println("Game::onShot: Player dead");
        notifyEvent(GameEventType::HIT_DEAD);
    }
}

void Game::notifyEvent(GameEventType eventType)
{
    for (int i = 0; i < listeners.size(); i++)
    {
        listeners.get(i)->onGameEvent(eventType);
    }
}

void Game::update()
{
    handleFire();
    notifyEvent(GameEventType::GAME_TICK);
}

void Game::setup(unsigned int teamId, unsigned int maxHealth, unsigned int maxAmmo, unsigned int maxCoolDown)
{
    // Try to create randomness and generate a player id
    randomSeed(analogRead(A0));
    playerId = random() % 1024;
    teamId = teamId;

    Serial.print("Game::setup: Starting as player #");
    Serial.print(playerId);
    Serial.print(" on team ");
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
        Serial.print("Game::onShot: Received a shot from ");
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

unsigned int Game::getPlayerId()
{
    return playerId;
}

unsigned int Game::getMaxHealth()
{
    return maxHealth;
}

unsigned int Game::getCurrentCoolDown()
{
    if (lastFireTime + maxCoolDown < millis())
    {
        return 0;
    }
    return lastFireTime + maxCoolDown - millis();
}

unsigned int Game::getMaxCoolDown()
{
    return maxCoolDown;
}

void Game::addListener(GameListener *listener)
{
    listeners.add(listener);
}

unsigned int Game::getCurrentHealth() {
    return health;
}