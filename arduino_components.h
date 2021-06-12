#ifndef ARDUINO_COMPONENTS_H
#define ARDUINO_COMPONENTS_H

#ifdef ARDUINO

#include "components.h"

class PinMode : public Node {
public:
    Input<uint8_t> pin;
    Input<uint8_t> mode;
private:
    bool isFireable() override() { return pin.isFull() && mode.isFull(); }
    void process() { pinMode(pin.get(), mode.get() };
};

class DigitalWrite : public Node {
public:
    Input<uint8_t> pin;
    Input<uint8_t> val;
private:
    bool isFireable() override() { return pin.isFull() && val.isFull(); }
    void process() { digitalWrite(pin.get(), val.get() };
};

class DigitalRead : public Node {
public:
    Input<uint8_t> pin;
    Output<uint8_t> out;
private:
    bool isFireable() override() { return pin.isFull() && val.isFull(); }
    void process() { out.put(digitalRead(pin.get()) };
};

class Every : public Node {
    Output<Bang> out;
public:
    Every(unsigned long period) : period(period) { }
private:
    unsigned long tStart = millis();
    bool isFireable() { return true }
    void process() {
        if(millis() - tStart > period) {
            tStart = millis();
            out.put(Bang());
        }
    }
    unsigned long period;
};


#endif

#endif