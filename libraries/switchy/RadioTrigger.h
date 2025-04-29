#ifndef RadioTrigger_h
#define RadioTrigger_h

#include "Config.h"
#include "RadioSignal.h"

class RadioTrigger {
public:
    // === Setup the RF receiver ===
    void Setup() {
        receiver.enableReceive(Config::Radio::ReceiverIrq);
    }

    // === Check if signal state changed since last Evaluate() ===
    bool HasChanged() {
        if (!hasChanged) return false;
        hasChanged = false;
        return true;
    }

    // === Get the last known signal state (true = ON) ===
    bool IsOn() const { return isOn; }

    // === Main logic loop: checks for new signals ===
    void Evaluate() {
        if (!receiver.available()) return;

        RadioSignal signal = RadioSignal::Create(&receiver);
        
        if (!signal.IsValid()) {
            signal.Print(); // Show invalid signal for debugging
            return;
        }

        if (IsRepeatedSignal(signal)) return;
        
        if (!signal.IsHardcoded() && IsDuplicateSignal(signal)) return;

        signal.Print(); // Show valid, accepted signal

        if (isOn != signal.IsOn) {
            isOn = signal.IsOn;
            hasChanged = true;
        }
    }

private:
    // === Radio receiver instance ===
    RCSwitch receiver = RCSwitch();

    // === Recent payload memory for deduplication ===
    unsigned long payloadMemory[Config::Radio::MemorySize] = { 0 };
    uint8_t payloadMemoryIndex = 0;
    unsigned long previousPayload = 0;

    // === Internal state ===
    bool isOn = false;
    bool hasChanged = false;

    // === Filter: ignore exact same signal repeated back-to-back ===
    bool IsRepeatedSignal(const RadioSignal& signal) {
        if (previousPayload == signal.Payload) return true;
        previousPayload = signal.Payload;
        return false;
    }

    // === Filter: ignore signals seen within the memory window ===
    bool IsDuplicateSignal(const RadioSignal& signal) {
        for (uint8_t i = 0; i < Config::Radio::MemorySize; ++i) {
            if (payloadMemory[i] == signal.Payload) {
                return true;
            }
        }

        payloadMemory[payloadMemoryIndex] = signal.Payload;
        payloadMemoryIndex = (payloadMemoryIndex + 1) % Config::Radio::MemorySize;

        return false;
    }
};

#endif