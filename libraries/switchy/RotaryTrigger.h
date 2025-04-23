#ifndef RotaryTrigger_h
#define RotaryTrigger_h

#include <Arduino.h>

#include "Config.h"

class RotaryTrigger {
public:
    // === Initialize the rotary input pin ===
    void Setup() {
        pinMode(Config::Rotary::ClockPin, INPUT_PULLUP);
    }

    // === Check if rotary was triggered since last check ===
    bool HasChanged() {
        if (!didTrigger) return false;
        didTrigger = false;
        return true;
    }

    // === Called from ISR to debounce + register a rotary tick ===
    void HandleInterrupt() {
        unsigned long currentTime = millis();

        // Debounce logic to ignore rapid repeats
        if (currentTime - lastTrigger > Config::Rotary::Debounce) {
            didTrigger = true;
            lastTrigger = currentTime;
        }
    }

private:
    // === Internal state ===
    volatile bool didTrigger = false; // Flag set from interrupt
    unsigned long lastTrigger = 0;    // Time of last valid trigger
};

#endif