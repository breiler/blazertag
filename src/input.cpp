#include <Arduino.h>
#include <IRremote.h>
#include "input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::addListener(InputListener *listener)
{
    listeners.add(listener);
}

void Input::setup(unsigned int irReceivePin, unsigned int triggerPin)
{
    this->irReceivePin = irReceivePin;
    this->triggerPin = triggerPin;

    Serial.print("Input::setup: Initializing IR feedback on pin ");
    Serial.println(LED_BUILTIN);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.print("Input::setup: Initializing IR reciver on pin ");
    Serial.println(irReceivePin);
    IrReceiver.begin(irReceivePin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // Start the receiver, enable feedback LED, take LED feedback pin from the internal boards definition

    Serial.print("Input::setup: Initialization trigger on pin ");
    Serial.println(triggerPin);
    pinMode(triggerPin, INPUT_PULLUP);

    Serial.println("Input::setup: Initialization complete");
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
        if (IrReceiver.decodedIRData.decodedRawData != 0)
        {
            notifyShot(IrReceiver.decodedIRData.decodedRawData);
        }
        IrReceiver.resume();
    }
}

void Input::notifyShot(unsigned int code)
{
    for (int i = 0; i < listeners.size(); i++)
    {
        listeners.get(i)->onReceivedShot(code);
    }
}

void Input::notifyFiring()
{
    for (int i = 0; i < listeners.size(); i++)
    {
        listeners.get(i)->onFire();
    }
}

void Input::notifyStopFiring()
{
    for (int i = 0; i < listeners.size(); i++)
    {
        listeners.get(i)->onStopFire();
    }
}