#pragma once
#include <iostream>

#include "src/capabilities/onoff.h"
#include "src/devices/devices.h"
class BasicLight : public Device, public OnOff {
 public:
  using Device::Device;
  virtual ~BasicLight() = default;
};
