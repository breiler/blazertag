#ifndef __INPUT_H
#define __INPUT_H

#include <LinkedList.h>
#include <IRremoteInt.h>
#include "inputlistener.h"

class Input {
    private:
        unsigned int irReceivePin;
        unsigned int triggerPin;

        unsigned long lastUpdateTime;
        bool isFiring;

        LinkedList<InputListener*> listeners;

        void readIR();
        void readTrigger();

        void notifyShot(unsigned int code);
        void notifyFiring();
        void notifyStopFiring();

    public: 
        Input();
        ~Input();
        void setup(unsigned int irReceivePin, unsigned int triggerPin);
        void update();
        void addListener(InputListener* listener);
};

#endif