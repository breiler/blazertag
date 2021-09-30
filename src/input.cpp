#include <Arduino.h>
#include <IRremote.h>
#include "input.h"

Input::Input()
{
    for (uint8_t i = 0; i < NUMBER_OF_INPUT_LISTENERS; i++)
    {
        listeners[i] = 0;
    }
}

Input::~Input()
{
}

void Input::addListener(InputListener *listener)
{
    for (uint8_t i = 0; i < NUMBER_OF_INPUT_LISTENERS; i++)
    {
        if (listeners[i] == 0)
        {
            listeners[i] = listener;
            return;
        }
    }
}

void Input::setup(uint8_t irReceivePin, uint8_t triggerPin)
{
    this->irReceivePin = irReceivePin;
    this->triggerPin = triggerPin;

    Serial.print(F("Input::setup: Initializing IR feedback on pin "));
    Serial.println(LED_BUILTIN);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.print(F("Input::setup: Initializing IR reciver on pin "));
    Serial.println(irReceivePin);
    IrReceiver.begin(irReceivePin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // Start the receiver, enable feedback LED, take LED feedback pin from the internal boards definition

    Serial.print(F("Input::setup: Initialization trigger on pin "));
    Serial.println(triggerPin);
    pinMode(triggerPin, INPUT_PULLUP);

    Serial.println(F("Input::setup: Initialization complete"));
}

void Input::update()
{
    readIR();
    readTrigger();
}

void Input::readTrigger()
{
    bool triggerReading = digitalRead(triggerPin);
    bool isCurrentlyFiring = triggerReading == LOW;

    // If the firing state changed
    if (isCurrentlyFiring != isFiring)
    {
        if (isCurrentlyFiring)
        {
            notifyFiring();
            isFiring = true;
        }
        else if (!isCurrentlyFiring)
        {
            notifyStopFiring();
            isFiring = false;
        }
    }
}

void Input::readIR()
{
    if (IrReceiver.decode())
    {
        if (IrReceiver.decodeRC6())
        {
            Serial.print("r: ");
            Serial.print(IrReceiver.decodedIRData.decodedRawData);
            Serial.print(" c: ");
            Serial.print(IrReceiver.decodedIRData.command);
            Serial.print(" p: ");
            Serial.print(IrReceiver.decodedIRData.protocol);
            Serial.print(" a: ");
            Serial.println(IrReceiver.decodedIRData.address);

            if (IrReceiver.decodedIRData.command != 0)
            {
                notifyShot(IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command);
            }
        }
        IrReceiver.resume();
    }
}

void Input::notifyShot(uint8_t teamId, uint8_t playerId)
{
    for (int i = 0; i < NUMBER_OF_INPUT_LISTENERS; i++)
    {
        if (listeners[i] != 0)
        {
            listeners[i]->onReceivedShot(teamId, playerId);
        }
    }
}

void Input::notifyFiring()
{
    for (int i = 0; i < NUMBER_OF_INPUT_LISTENERS; i++)
    {
        if (listeners[i] != 0)
        {
            listeners[i]->onFire();
        }
    }
}

void Input::notifyStopFiring()
{
    for (int i = 0; i < NUMBER_OF_INPUT_LISTENERS; i++)
    {
        if (listeners[i] != 0)
        {
            listeners[i]->onStopFire();
        }
    }
}