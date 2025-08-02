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
            UpdateState(!globalOn);
            return;
        }

        // 3. Radio-triggered update
        radioTrigger->Evaluate();
        if (radioTrigger->HasChanged()) {
            UpdateState();
        }
    }

    // === Get current ON/OFF state ===
    bool Unit1IsOn() const { return unit1On; }
    bool Unit2IsOn() const { return unit2On; }
    bool Unit3IsOn() const { return unit3On; }
    bool Unit4IsOn() const { return unit4On; }

    // === Check if state changed since last Evaluate() ===
    bool StateChanged() {
        if (!stateChanged) return false;
        stateChanged = false;
        return true;
    }

private:
    RadioTrigger* radioTrigger = nullptr;
    RotaryTrigger* rotaryTrigger = nullptr;
    
    bool unit1On = false; 
    bool unit2On = false; 
    bool unit3On = false; 
    bool unit4On = false; 
    bool globalOn = false;
    
    bool stateChanged = false;

    bool delayActive = false;
    unsigned long delayUntil = 0;

    // === Central state update handler ===
    void UpdateState() {
        
        if (radioTrigger->Target == Config::Unit::TargetGlobal) {
            if(radioTrigger->State == Config::State::Toggle) {
                unit1On = unit2On = unit3On = unit4On = globalOn = !globalOn;
            }
            if(radioTrigger->State == Config::State::On) {
                unit1On = unit2On = unit3On = unit4On = globalOn = true;
            }
            if(radioTrigger->State == Config::State::Off) {
                unit1On = unit2On = unit3On = unit4On = globalOn = false;
            }
            stateChanged = true;
        }
        
        if (radioTrigger->Target == Config::Unit::Target1) {
            if(radioTrigger->State == Config::State::Toggle) {
                unit1On = !unit1On;
                stateChanged = true;
            }
            if(radioTrigger->State == Config::State::On && !unit1On) {
                unit1On = true;
                stateChanged = true;
            }
            if(radioTrigger->State == Config::State::Off && unit1On) {
                unit1On = false;
                stateChanged = true;
            }
        }
        
        if (radioTrigger->Target == Config::Unit::Target2) {
            if(radioTrigger->State == Config::State::Toggle) {
                unit2On = !unit2On;
                stateChanged = true;
            }
            if(radioTrigger->State == Config::State::On && !unit2On) {
                unit2On = true;
                stateChanged = true;
            }
            if(radioTrigger->State == Config::State::Off && unit2On) {
                unit2On = false;
                stateChanged = true;
            }
        }
        
        if (radioTrigger->Target == Config::Unit::Target3) {
            if(radioTrigger->State == Config::State::Toggle) {
                unit3On = !unit3On;
                stateChanged = true;
            }
            if(radioTrigger->State == Config::State::On && !unit3On) {
                unit3On = true;
                stateChanged = true;
            }
            if(radioTrigger->State == Config::State::Off && unit3On) {
                unit3On = false;
                stateChanged = true;
            }
        }
        
        if (radioTrigger->Target == Config::Unit::Target4) {
            if(radioTrigger->State == Config::State::Toggle) {
                unit4On = !unit4On;
                stateChanged = true;
            }
            if(radioTrigger->State == Config::State::On && !unit4On) {
                unit4On = true;
                stateChanged = true;
            }
            if(radioTrigger->State == Config::State::Off && unit4On) {
                unit4On = false;
                stateChanged = true;
            }
        }

        // if (newState) {
        //     CancelDelay();
        //     stateChanged = true;
        // } else {
        //     StartDelay();
        // }
    }
    
    void UpdateState(bool on) {
        unit1On = unit2On = unit3On = unit4On = globalOn = on;
        stateChanged = true;
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