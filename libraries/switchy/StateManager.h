#ifndef StateManager_h
#define StateManager_h

#include "RotaryTrigger.h"
#include "RadioTrigger.h"
#include "Config.h"

class StateManager {
public:
    // === Setup the state manager with external triggers ===
    void Setup(RadioTrigger* radio, RotaryTrigger* rotary) {
        radioTrigger = radio;
        rotaryTrigger = rotary;
    }

    // === Main evaluation function, called in loop() ===
    void Evaluate() {
        // 1. Delayed OFF handling
        if (DelayFinished()) {
            stateChanged = true;
            return;
        }

        // 2. Rotary toggle
        if (rotaryTrigger->HasChanged()) {
            UpdateState(!currentStateOn);
            return;
        }

        // 3. Radio-triggered update
        radioTrigger->Evaluate();
        if (radioTrigger->HasChanged()) {
            UpdateState(radioTrigger->IsOn());
        }
    }

    // === Get current ON/OFF state ===
    bool IsOn() const {
        return currentStateOn;
    }

    // === Check if state changed since last Evaluate() ===
    bool StateChanged() {
        if (!stateChanged) return false;
        stateChanged = false;
        return true;
    }

private:
    RadioTrigger* radioTrigger = nullptr;
    RotaryTrigger* rotaryTrigger = nullptr;

    bool currentStateOn = false;
    bool stateChanged = false;

    bool delayActive = false;
    unsigned long delayUntil = 0;

    // === Central state update handler ===
    void UpdateState(bool newState) {
        if (newState == currentStateOn) return;

        currentStateOn = newState;

        if (newState) {
            CancelDelay();
            stateChanged = true;
        } else {
            StartDelay();
        }
    }

    void StartDelay() {
        delayActive = true;
        delayUntil = millis() + Config::State::DelayMs;
    }

    void CancelDelay() {
        delayActive = false;
    }

    bool DelayFinished() {
        if (delayActive && millis() > delayUntil) {
            CancelDelay();
            return true;
        }
        return false;
    }
};

#endif