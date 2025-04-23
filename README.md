# RF State Control (Arduino)

This project implements a simple unidirectional RF communication system between two Arduino nodes.

One device (the sender) transmits ON/OFF signals based on power state.  
The other (the receiver) listens for signals, filters out duplicates, and controls one or more relays based on the received state.

The code is modular and event-driven. Manual control is possible through an optional rotary input. RF signal validation includes embedded identifiers and basic replay protection.

## Structure
```
Sender/
 Sender.ino (Sender firmware)
Receiver/
 Receiver.ino (Receiver firmware)
Libraries/switchy/ (Shared library)
 Config.h
 Relay.h
 RadioSignal.h
 RadioTrigger.h
 RotaryTrigger.h
 StateManager.h
```

## Dependencies

- [RCSwitch](https://github.com/sui77/rc-switch)
- Arduino-compatible board (e.g. Nano, Uno, MKR)

## Hardware Used for This Project

- Arduino Nano (2x)
- 433 MHz RF modules:
  - **Transmitter**: WL102-341
  - **Receiver**: RX470-4
  
## Notes

- Configuration is centralized in `Config.h`
- All RF signals are stateless and encoded with an embedded ID
- Relay state changes are handled by a state machine with optional delay logic
- Debugging and testing tools (e.g. manual override) can be enabled via `#define DEBUG` 

## License

This project is licensed under the GNU GPL v3.  
See the [LICENSE](./LICENSE) file for details.