#ifndef Relay_h
#define Relay_h

#include <Arduino.h>

class Relay {
public:
    // === Setup the relay: assign pin and optional name ===
    void Setup(uint8_t relayPin, const char* relayName = nullptr) {
        pin = relayPin;
        name = relayName;
        pinMode(pin, OUTPUT);
        TurnOff(); // Ensure known initial state
    }

    // === Check current state ===
    bool IsOn() const {
        return isOn;
    }

    // === Turn the relay ON ===
    void TurnOn() {
        isOn = true;
        digitalWrite(pin, HIGH);
        PrintState("ON");
    }

    // === Turn the relay OFF ===
    void TurnOff() {
        isOn = false;
        digitalWrite(pin, LOW);
        PrintState("OFF");
    }

    // === Toggle relay state ===
    void Toggle() {
        if (isOn) {
            TurnOff();
        } else {
            TurnOn();
        }
    }

private:
    uint8_t pin = 0;
    const char* name = nullptr;
    bool isOn = false;

    void PrintState(const char* state) const {
        if (name) {
            Serial.print(name);
            Serial.print(" ");
        }
        Serial.println(state);
    }
};

#endif