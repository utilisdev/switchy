#ifndef RadioTrigger_h
#define RadioTrigger_h

#include "Config.h"
#include "RadioSignal.h"

class RadioTrigger {
public:

    uint8_t Target = Config::Unit::TargetGlobal;
    uint8_t State = Config::State::Off;

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
    
    // === Main logic loop: checks for new signals ===
    void Evaluate() {
        if (!receiver.available()) { return; }
        
        RadioSignal signal = RadioSignal::Create(&receiver);
        
        if (!IsValidSignal(signal)) {
            signal.Print(); // Show invalid signal for debugging
            return;
        }

        if (IsRepeatedSignal(signal)) { return; }
        
        if (Target == Config::Unit::TargetGlobal && IsDuplicateSignal(signal)) {
            return;
        }

        signal.Print(); // Show valid, accepted signal

        hasChanged = true;
    }

private:
    // === Radio receiver instance ===
    RCSwitch receiver = RCSwitch();

    // === Recent payload memory for deduplication ===
    unsigned long payloadMemory[Config::Radio::MemorySize] = { 0 };
    uint8_t payloadMemoryIndex = 0;
    unsigned long previousPayload = 0;
    unsigned long debounce = 0;
    

    // === Internal state ===
    bool hasChanged = false;

    // === Filter: ignore exact same signal repeated back-to-back ===
    bool IsRepeatedSignal(const RadioSignal& signal) {
        bool isRepeated = previousPayload == signal.Payload && millis() < debounce;
        previousPayload = signal.Payload;
        debounce = millis() + Config::Radio::Debounce;
        return isRepeated;
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
    
    bool IsValidSignal(const RadioSignal& signal) {
        if (!signal.IsValid()) { return false; }
        
        if(signal.Payload == Config::Unit::UnitId1) { Target = Config::Unit::Target1; } else
        if(signal.Payload == Config::Unit::UnitId2) { Target = Config::Unit::Target2; } else
        if(signal.Payload == Config::Unit::UnitId3) { Target = Config::Unit::Target3; } else
        if(signal.Payload == Config::Unit::UnitId4) { Target = Config::Unit::Target4; } else
        { Target = Config::Unit::TargetGlobal; }
        
        if(signal.Channel == Config::Radio::ChannelOff) { State = Config::State::Off; }
        if(signal.Channel == Config::Radio::ChannelOn) { State = Config::State::On; }
        if(signal.Channel == Config::Radio::ChannelToggle) { State = Config::State::Toggle; }
        
        return true;
    }
};

#endif