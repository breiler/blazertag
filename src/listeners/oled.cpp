#include "listeners/oled.h"

Oled::Oled()
{
}

void Oled::setup(Game *game)
{
    this->game = game;

    // The I2C SDA/SCL pins set to -1 means to use the default Wire library
    // If pins were specified, they would be bit-banged in software
    // This isn't inferior to hw I2C and in fact allows you to go faster on certain CPUs
    // The reset pin is optional and I've only seen it needed on larger OLEDs (2.4")
    //    that can be configured as either SPI or I2C
    //
    // oledInit(SSOLED *, type, oled_addr, rotate180, invert, bWire, SDA_PIN, SCL_PIN, RESET_PIN, speed)
    int rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
    if (rc != OLED_NOT_FOUND)
    {
        char *msgs[] = {(char *)"SSD1306 @ 0x3C", (char *)"SSD1306 @ 0x3D", (char *)"SH1106 @ 0x3C", (char *)"SH1106 @ 0x3D"};
        oledFill(&ssoled, 0, 1);
        oledWriteString(&ssoled, 0, 0, 0, msgs[rc], FONT_NORMAL, 0, 1);
        oledSetBackBuffer(&ssoled, ucBackBuffer);
    }

    lastUpdateTime = millis();
    displayState = DisplayState::DISPLAY_STATUS;
    displayStatus(true, true);
}

void Oled::onGameEvent(GameEventType gameEventType)
{
    if (gameEventType == GameEventType::HIT ||
        gameEventType == GameEventType::FIRING_SHOT)
    {
        if (game->getCurrentHealth() > 0 && game->getCurrentAmmo())
        {
            displayState = DisplayState::DISPLAY_STATUS;
            displayStatus(gameEventType == GameEventType::HIT, gameEventType == GameEventType::FIRING_SHOT);
            lastUpdateTime = millis();
        }
        else
        {
            displayState = DisplayState::DISPLAY_GAME_OVER;
            displayGameOver(true);
            lastUpdateTime = millis();
        }
    }
    else if (gameEventType == GameEventType::GAME_OVER ||
             gameEventType == GameEventType::HIT_DEAD ||
             gameEventType == GameEventType::FIRING_SHOT_NO_AMMO ||
             gameEventType == GameEventType::FIRING_SHOT_NO_HEALTH)
    {
        displayState = DisplayState::DISPLAY_GAME_OVER;
        displayGameOver(true);
        lastUpdateTime = millis();
    }
    else if (gameEventType == GAME_TICK &&
             lastUpdateTime != 0 &&
             lastUpdateTime + 1000 < millis())
    {
        lastUpdateTime = 0;
        if (displayState == DisplayState::DISPLAY_STATUS)
        {
            displayStatus(false, false);
        }
        else if (displayState == DisplayState::DISPLAY_GAME_OVER)
        {
            displayGameOver(false);
        }
    }
}

void Oled::displayStatus(bool inverseHealth, bool inverseAmmo)
{
    oledFill(&ssoled, 0x0, 1);
    oledWriteString(&ssoled, 0, 28, 0, (char *)"BLAZERTAG", FONT_NORMAL, 0, 1);

    oledWriteString(&ssoled, 0, 24, 3, (char *)"H: ", FONT_STRETCHED, 0, 1);
    oledWriteString(&ssoled, 0, -1, 3, (char *)String(game->getCurrentHealth()).c_str(), FONT_STRETCHED, inverseHealth, 1);

    oledWriteString(&ssoled, 0, 24, 6, (char *)"A: ", FONT_STRETCHED, 0, 1);
    oledWriteString(&ssoled, 0, -1, 6, (char *)String(game->getCurrentAmmo()).c_str(), FONT_STRETCHED, inverseAmmo, 1);
}

void Oled::displayGameOver(bool inverse)
{
    oledFill(&ssoled, 0x0, 1);
    oledWriteString(&ssoled, 0, 0, 3, (char *)"GameOver", FONT_STRETCHED, inverse, 1);
}