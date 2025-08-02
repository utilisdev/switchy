//#define DEBUG
//#define DEVICE_1
//#define DEVICE_2
#define DEVICE_3

#include <RCSwitch.h>

#include "Config.h"
#include "StateManager.h"
#include "RotaryTrigger.h"
#include "RadioTrigger.h"
#include "Relay.h"


// === RF Receiver ===
RCSwitch receiver = RCSwitch();

// === Modules ===
StateManager stateManager;
RotaryTrigger rotaryTrigger;
RadioTrigger radioTrigger;
Relay relay1;
Relay relay2;

void setup() {

  #ifdef DEBUG
    Serial.begin(9600);
  #endif

  // Setup input modules
  rotaryTrigger.Setup();
  radioTrigger.Setup();

  // Setup output relays
  relay1.Setup(Config::Relay::Relay1Pin, "Relay 1");
  #ifdef DEVICE_3
    relay2.Setup(Config::Relay::Relay2Pin, "Relay 2");
  #endif
  

  // Setup state manager with trigger sources
  stateManager.Setup(&radioTrigger, &rotaryTrigger);

  // Attach interrupt for rotary input 
  attachInterrupt(
    digitalPinToInterrupt(Config::Rotary::ClockPin),
    []() { rotaryTrigger.HandleInterrupt(); },
    RISING
  );
}

void loop() {
  stateManager.Evaluate();

  if (!stateManager.StateChanged()) return;

  #ifdef DEVICE_1
    if (stateManager.Unit1IsOn()) {
      relay1.TurnOn();
    } else {
      relay1.TurnOff();
    }
  #endif
  
  #ifdef DEVICE_2
    if (stateManager.Unit2IsOn()) {
      relay1.TurnOn();
    } else {
      relay1.TurnOff();
    }
  #endif

  #ifdef DEVICE_3
    if (stateManager.Unit3IsOn()) {
      relay1.TurnOn();
    } else {
      relay1.TurnOff();
    }

    if (stateManager.Unit4IsOn()) {
      relay2.TurnOn();
    } else {
      relay2.TurnOff();
    }
  #endif
  
}
