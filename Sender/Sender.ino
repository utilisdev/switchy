//#define DEBUG
//#define REMOTE_CTRL_SETUP
//#define DEVICE_1
//#define DEVICE_2
//#define DEVICE_3

#include <RCSwitch.h>

#include "Config.h"
#include "RotaryTrigger.h"
#include "RadioSignal.h"


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
  
  delay(200);

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
  delay(200);
  static RadioSignal sig = RadioSignal::Create(Config::Radio::ChannelToggle);
  sig.Payload = Config::Unit::UnitId3; 
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
  RadioSignal::Create(Config::Radio::ChannelOn).Send(&sender);
}

// === Send OFF Signal ===
void SendOff() {
  isOn = false;
  digitalWrite(Config::State::LedPin, LOW);
  RadioSignal::Create(Config::Radio::ChannelOff).Send(&sender);
}