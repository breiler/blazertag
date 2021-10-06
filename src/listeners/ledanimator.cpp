#include <Arduino.h>
#include <FastLED.h>
#include "listeners/ledanimator.h"
#include "gameeventtype.h"

// How many leds in your strip?
#define NUM_LEDS 2

#ifdef ARDUINO_ARCH_ESP32 
#define DATA_PIN 22
#define CLOCK_PIN 23
#else 
#define DATA_PIN 9
#define CLOCK_PIN 8
#endif

// Define the array of leds
CRGB leds[NUM_LEDS];

#define BLINK_INTERVAL 2000

LedAnimator::LedAnimator()
{
    lastUpdateTime = millis();
    animationType = AnimationType::ANIMATION_IDLE;
}

void LedAnimator::setup(Game *game)
{
    FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear(true);
    this->game = game;
}

void LedAnimator::handleGameOverAnimation()
{
    int interval = BLINK_INTERVAL / 6;
    float currentFrame = (float)((lastUpdateTime + interval) - millis()) / (float)interval / 2;
    if (currentFrame < 0 || currentFrame > interval / 2)
    {
        lastUpdateTime = millis();
        return;
    }

    float factor = 0;
    if (currentFrame < 1)
    {
        factor = currentFrame;
    }
    else
    {
        factor = 1 - (currentFrame - 1);
    }

    leds[0].setRGB(255 * factor, 0, 0);

    FastLED.show();
}

void LedAnimator::handleIdleAnimation()
{
    float currentFrame = (float)((lastUpdateTime + BLINK_INTERVAL * 2) - millis()) / (float)BLINK_INTERVAL;
    if (currentFrame < 0 || currentFrame > BLINK_INTERVAL)
    {
        lastUpdateTime = millis();
        return;
    }

    float factor = 0;
    if (currentFrame < 1)
    {
        factor = currentFrame;
    }
    else
    {
        factor = 1 - (currentFrame - 1);
    }

    if (game->getCurrentHealth() == 0)
    {
        leds[0].setRGB(255 * factor, 0, 0);
    }
    else
    {
        leds[0].setRGB(53 * factor, 133 * factor, 179 * factor);
    }

    FastLED.show();
}

void LedAnimator::handleHitAnimation()
{
    float interval = BLINK_INTERVAL / 3;
    float currentFrame = (float)((lastUpdateTime + interval) - millis()) / (interval / 2);
    if (currentFrame < 0 || currentFrame > interval)
    {
        lastUpdateTime = millis();
        leds[0] = CRGB::Black;
        animationType = AnimationType::ANIMATION_IDLE;
        return;
    }

    if (currentFrame < 1)
    {
        leds[0].setRGB((255 * currentFrame) + 1, 0, 20 * currentFrame);
    }
    else
    {
        leds[0].setRGB((255 - (255 * (currentFrame - 1))), 0, 20 - (20 * (currentFrame - 1)));
    }

    FastLED.show();
}

void LedAnimator::handleFireAnimation()
{
    if (game->getCurrentCoolDown() <= 0)
    {
        leds[1] = CRGB::Black;
        animationType = AnimationType::ANIMATION_IDLE;
        return;
    }

    // Calculate how much time has passed in the fire sequence by using the fire cooldown
    float fireAnimationFrame = ((float)game->getCurrentCoolDown() / (float)game->getMaxCoolDown());
    leds[1].green = (uint8_t)128 * fireAnimationFrame;
    FastLED.show();
}

void LedAnimator::onGameEvent(GameEventType gameEventType)
{
    if (gameEventType == GameEventType::FIRING_SHOT)
    {
        Serial.println(F("LedAnimator::onGameEvent: Starting animation FIRE"));
        lastUpdateTime = millis();
        this->animationType = AnimationType::ANIMATION_FIRE;
    }

    if (gameEventType == GameEventType::HIT)
    {
        Serial.println(F("LedAnimator::onGameEvent: Starting animation HIT"));
        lastUpdateTime = millis();
        this->animationType = AnimationType::ANIMATION_HIT;
    }

    if (gameEventType == GameEventType::HIT_DEAD || gameEventType == GameEventType::HIT_KILLED || gameEventType == GameEventType::GAME_OVER)
    {
        Serial.println(F("LedAnimator::onGameEvent: Starting animation GAME OVER"));
        lastUpdateTime = millis();
        this->animationType = AnimationType::ANIMATION_GAME_OVER;
    }

    // Render with 20 FPS (1000ms/20fps = 50ms/frame)
    if (millis() - lastAnimationFrameTime > 50)
    {
        lastAnimationFrameTime = millis();
        if (animationType == AnimationType::ANIMATION_FIRE)
        {
            handleFireAnimation();
        }
        else if (animationType == AnimationType::ANIMATION_IDLE)
        {
            handleIdleAnimation();
        }
        else if (animationType == AnimationType::ANIMATION_HIT)
        {
            handleHitAnimation();
        }
        else if (animationType == AnimationType::ANIMATION_GAME_OVER)
        {
            handleGameOverAnimation();
        }
    }
}