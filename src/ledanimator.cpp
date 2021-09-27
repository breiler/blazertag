#include <Arduino.h>
#include <FastLED.h>
#include "ledanimator.h"

// How many leds in your strip?
#define NUM_LEDS 2
#define DATA_PIN 9
#define CLOCK_PIN 8

// Define the array of leds
CRGB leds[NUM_LEDS];

#define BLINK_INTERVAL 2000

LedAnimator::LedAnimator()
{
    lastUpdateTime = millis();
    FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear(true);

    animationType = AnimationType::ANIMATION_IDLE;
}

void LedAnimator::setup(Game *game)
{
    this->game = game;
}

void LedAnimator::handleIdleAnimation()
{
    float currentFrame = (float)((lastUpdateTime + BLINK_INTERVAL * 2) - millis()) / (float)BLINK_INTERVAL;
    if (currentFrame < 0 || currentFrame > BLINK_INTERVAL)
    {
        lastUpdateTime = millis();
        return;
    }

    leds[0].red = 0;
    leds[0].green = 0;

    int color = 0;
    if (currentFrame < 1)
    {
         color = (40 * currentFrame) + 1;
    }
    else
    {
        color = (40 - (40 * (currentFrame - 1))) + 1;
    }

    if(game->getCurrentHealth() == 0) {
        leds[0].red = color;
    } else {
        leds[0].green = color;
    }

    FastLED.show();
}

void LedAnimator::handleHitAnimation()
{
    float interval = BLINK_INTERVAL/2;
    float currentFrame = (float)((lastUpdateTime + interval) - millis()) / (interval/2);
    if (currentFrame < 0 || currentFrame > interval)
    {
        lastUpdateTime = millis();
        leds[0] = CRGB::Black;
        animationType = AnimationType::ANIMATION_IDLE;
        return;
    }

    leds[0].green = 0;

    if (currentFrame < 1)
    {
        // Turn the LED on, then pause
        leds[0].red = (127 * currentFrame) + 1;
    }
    else
    {
        // Now turn the LED off, then pause
        leds[0].red = (127- (127 * (currentFrame - 1)));
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

void LedAnimator::onGameEvent(GameEventType gameEvent)
{
    if (gameEvent == GameEventType::FIRING_SHOT)
    {
        Serial.println("LedAnimator::onGameEvent: Starting animation FIRE");
        lastUpdateTime = millis();
        this->animationType = AnimationType::ANIMATION_FIRE;
    }

    if (gameEvent == GameEventType::HIT)
    {
        Serial.println("LedAnimator::onGameEvent: Starting animation HIT");
        lastUpdateTime = millis();
        this->animationType = AnimationType::ANIMATION_HIT;
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
    }
}