#ifndef __INPUTLISTENER_H
#define __INPUTLISTENER_H

class InputListener {
    public: 
        virtual void onFire() = 0;
        virtual void onStopFire() = 0;
        virtual void onReceivedShot(uint32_t code) = 0;
};

#endif