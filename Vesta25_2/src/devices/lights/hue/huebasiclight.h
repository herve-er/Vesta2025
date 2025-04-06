#pragma once
#include "src/devices/huebridge/huebridge.h"
#include "src/devices/lights/basiclight.h"

class hueBasicLight : public BasicLight {
 public:
  using BasicLight::BasicLight;
  ~hueBasicLight() override = default;
  void setBridgeId(DeviceId id);
  void setLightHueId(int64_t id);
  int64_t getBridgeId() const { return _bridgeId; }
  ResultSetState setState(State);
  ResultSwitch switchState();
  ResultGetState getState();

 private:
  std::shared_ptr<HueBridge> _bridge;
  DeviceId _bridgeId = 0;
  int64_t _hueId = 0;
};
