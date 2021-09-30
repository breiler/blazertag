#include <Arduino.h>
#include "listeners/irsender.h"

IRSender::IRSender()
{
}

void IRSender::setup(Game *game)
{
    this->game = game;
    IrSender.begin(IR_SEND_PIN, true);
}

void IRSender::onGameEvent(GameEventType gameEventType)
{
    if (gameEventType == GameEventType::FIRING_SHOT)
    {
        IrSender.sendRC6(game->getTeamId(), game->getPlayerId(), false);
    }
}