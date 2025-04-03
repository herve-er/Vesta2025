#pragma once
#include "capability.h"

class OnOff : public Capability {
  static inline constexpr char capabilityName[] = "OnOff";
  static inline constexpr char capabilityDesc[] =
      "The device can be turned on, off, switched and queried for its state.";

 public:
  enum class State { on, off };

 protected:
  OnOff() : Capability(capabilityName, capabilityDesc) {}
  enum class ResultSetState {
    turnedOn,
    turnedOff,
    alreadyOn,
    alreadyOff,
    error,
  };
  enum class ResultSwitch {
    turnedOn,
    turnedOff,
    error,
  };
  enum class ResultGetState {
    isOn,
    isOff,
    error,
  };

  virtual ResultSetState setState(State) = 0;
  virtual ResultSwitch switchState() = 0;
  virtual ResultGetState getState() = 0;

 public:
};
