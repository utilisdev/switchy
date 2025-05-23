#ifndef RadioSignal_h
#define RadioSignal_h

#include <RCSwitch.h>

#include "Config.h"

class RadioSignal {
public:
    // === Public Members ===
    unsigned long Payload;
    unsigned long Channel;
    unsigned int Size = 32; // Default payload size in bits
    bool IsOn = false;

    bool IsValid() const { return isValid; }
    
    bool IsHardcoded() {
        return (Payload == 14123777 || Payload == 7848590);
    }

    // === Factory: Generate new outgoing signal ===
    static RadioSignal Create(bool isOn = false) {
        RadioSignal signal;

        signal.IsOn = isOn;
        signal.Channel = isOn ? Config::Radio::ChannelOn : Config::Radio::ChannelOff;
        signal.Payload = GeneratePayload();
        signal.isValid = true;

        return signal;
    }

    // === Factory: Read received signal ===
    static RadioSignal Create(RCSwitch* receiver) {
        RadioSignal signal;
        signal.isValid = false;

        if (!receiver->available()) {
            return signal;
        }

        // Extract incoming signal info
        signal.Payload = receiver->getReceivedValue();
        signal.Size = receiver->getReceivedBitlength();
        signal.Channel = receiver->getReceivedProtocol();
        receiver->resetAvailable();

        signal.IsOn = (signal.Channel == Config::Radio::ChannelOn);
        signal.isValid = IsValidChannel(signal.Channel) && IsValidPayload(signal.Payload);

        return signal;
    }

    // === Send signal ===
    void Send(RCSwitch* sender) {
        if (!IsValid()) return;

        Print();

        sender->setProtocol(Channel);
        sender->send(Payload, Size);
    }

    // === Debug print ===
    void Print() const {
        Serial.print("Signal: ");
        Serial.print(Payload, HEX);
        Serial.print(" (");
        Serial.print(Payload);
        Serial.print(") / Bits: ");
        Serial.print(Size);
        Serial.print(" / Channel: ");
        Serial.print(Channel);
        Serial.print(" / Valid: ");
        Serial.print(isValid ? "True" : "False");
        Serial.print(" / On: ");
        Serial.println(IsOn ? "True" : "False");
    }

private:
    bool isValid = true;

    // === Internal helper: Check channel validity ===
    static bool IsValidChannel(unsigned long channel) {
        return channel == Config::Radio::ChannelOn || channel == Config::Radio::ChannelOff;
    }

    // === Internal helper: Check payload contains embedded ID ===
    static bool IsValidPayload(unsigned long payload) {
        for (uint8_t offset = 0; offset <= 16; ++offset) {
            uint8_t candidate = (payload >> offset) & 0xFF;
            if (candidate == Config::Radio::ComId) {
                return true;
            }
        }
        return false;
    }

    // === Internal helper: Generate a random payload with embedded ID ===
    static unsigned long GeneratePayload() {
        uint8_t bitOffset = random(0, 17); // 0–16 allows 8-bit insert
        unsigned long randSignal = random(0, 0xFFFFFF);

        // Clear 8 bits at the target offset
        unsigned long mask = ~(0xFFUL << bitOffset);
        randSignal &= mask;

        // Insert device ID
        randSignal |= ((unsigned long)Config::Radio::ComId << bitOffset);

        return randSignal;
    }
};

#endif