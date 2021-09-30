#ifndef __INPUT_H
#define __INPUT_H

#include <IRremoteInt.h>
#include "inputlistener.h"

#define NUMBER_OF_INPUT_LISTENERS 10

class Input {
    private:
        uint8_t irReceivePin;
        uint8_t triggerPin;

        unsigned long lastUpdateTime;
        bool isFiring;

        InputListener* listeners[NUMBER_OF_INPUT_LISTENERS];

        void readIR();
        void readTrigger();

        void notifyShot(uint8_t teamId, uint8_t playerId);
        void notifyFiring();
        void notifyStopFiring();

    public: 
        Input();
        ~Input();
        void setup(uint8_t irReceivePin, uint8_t triggerPin);
        void update();
        void addListener(InputListener* listener);
};

#endif