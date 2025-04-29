#include <RCSwitch.h>

#include "Config.h"
#include "RotaryTrigger.h"
#include "RadioSignal.h"

//#define DEBUG
//#define REMOTE_CTRL_SETUP

#ifdef DEBUG
RotaryTrigger rotaryTrigger;
bool dipOverrideOn = true;
#endif

// === RF Sender ===
RCSwitch sender = RCSwitch();

// === Internal State ===
bool isOn = false;

void setup() {
  
  #ifdef DEBUG
    Serial.begin(9600);
  #endif

  randomSeed(analogRead(A0)); // Seed for RF signal randomness

  pinMode(Config::State::LedPin, OUTPUT);
  pinMode(Config::State::PowerProbePin, INPUT);

  sender.enableTransmit(Config::Radio::SenderPin);

#ifdef DEBUG
  // Rotary input simulates DIP switch override in debug mode
  rotaryTrigger.Setup();
  attachInterrupt(
    digitalPinToInterrupt(Config::Rotary::ClockPin),
    []() { rotaryTrigger.HandleInterrupt(); },
    RISING
  );
#endif
}

void loop() {

#ifdef REMOTE_CTRL_SETUP
  // This is only used to send a signal continuously for setting-up a remote control.
  delay(random(0, 101));
  static RadioSignal sig = RadioSignal::Create(false);
  sig.Send(&sender);
  return;
#endif

  // === Check power state ===
  // If power is present → send ON signal
  // If power is lost → send OFF signal
  bool isPowered = digitalRead(Config::State::PowerProbePin);

#ifdef DEBUG
  // Override actual power state using rotary in debug mode
  isPowered = dipOverrideOn;

  if (rotaryTrigger.HasChanged()) {
    dipOverrideOn = !dipOverrideOn;
    isPowered = dipOverrideOn;
  }
#endif

  // Avoid duplicate signals if state hasn't changed
  if (isPowered == isOn) return;

  if (isPowered) {
    SendOn();
  } else {
    SendOff();
  }
}

// === Send ON Signal ===
void SendOn() {
  isOn = true;
  digitalWrite(Config::State::LedPin, HIGH);
  RadioSignal::Create(true).Send(&sender);
}

// === Send OFF Signal ===
void SendOff() {
  isOn = false;
  digitalWrite(Config::State::LedPin, LOW);
  RadioSignal::Create(false).Send(&sender);
}