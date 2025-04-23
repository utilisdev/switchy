#include <RCSwitch.h>

#include "Config.h"
#include "StateManager.h"
#include "RotaryTrigger.h"
#include "RadioTrigger.h"
#include "Relay.h"

// #define DEBUG

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
  relay2.Setup(Config::Relay::Relay2Pin, "Relay 2");

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

  if (!stateManager.IsOn()) {
    relay1.TurnOff();
    relay2.TurnOff();
    return;
  }

  // Toggle relays: relay2 is backup if relay1 is already on
  if (!relay1.IsOn()) {
    relay1.TurnOn();
  } else {
    relay2.TurnOn();
  }
}