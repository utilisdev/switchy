#ifndef Config_h
#define Config_h

#include <Arduino.h>

class Config {
public:
    // === Radio module configuration ===
    struct Radio {
        static constexpr uint8_t SenderPin           = A3;
        static constexpr uint8_t ReceiverPin         = 2;
        static constexpr uint8_t ReceiverIrq         = digitalPinToInterrupt(ReceiverPin);

        static constexpr uint8_t ComId               = 0xF0;
        static constexpr uint8_t ChannelOn           = 1;
        static constexpr uint8_t ChannelOff          = 2;
        static constexpr uint8_t ChannelToggle       = 5;
                
        static constexpr uint8_t MemorySize          = 100; // for deduplication
        
        static constexpr unsigned long Debounce      = 500; 
    };
    
    struct Unit {
        static constexpr unsigned long UnitId1       = 13106212;
        static constexpr unsigned long UnitId2       = 15986859;
        static constexpr unsigned long UnitId3       = 16525764;
        static constexpr unsigned long UnitId4       = 12330296;
        
        static constexpr uint8_t TargetGlobal         = 0;
        static constexpr uint8_t Target1              = 1;
        static constexpr uint8_t Target2              = 2;
        static constexpr uint8_t Target3              = 3;
        static constexpr uint8_t Target4              = 4;
    };

    // === Rotary input configuration ===
    struct Rotary {
        static constexpr uint8_t ClockPin            = 3;
        static constexpr uint8_t ClockIrq            = digitalPinToInterrupt(ClockPin);
        static constexpr unsigned long Debounce      = 1000; // ms
    };

    // === Relay pin configuration ===
    struct Relay {
        static constexpr uint8_t Relay1Pin           = 5;
        static constexpr uint8_t Relay2Pin           = 4;
    };

    // === State detection & LED ===
    struct State {
        static constexpr uint8_t PowerProbePin       = 2; // Watch out! Shares with ReceiverPin
        static constexpr unsigned long DelayMs       = 0; // Time window for override
        static constexpr uint8_t LedPin              = LED_BUILTIN;
        
        static constexpr uint8_t Off                 = 0;
        static constexpr uint8_t On                  = 1;
        static constexpr uint8_t Toggle              = 2;
    };
};

#endif