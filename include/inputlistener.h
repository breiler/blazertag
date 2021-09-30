#ifndef __INPUTLISTENER_H
#define __INPUTLISTENER_H

class InputListener {
    public: 
        virtual void onFire() = 0;
        virtual void onStopFire() = 0;
        virtual void onReceivedShot(uint8_t teamId, uint8_t playerId) = 0;
};

#endif