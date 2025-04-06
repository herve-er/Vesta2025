#include "huebasiclight.h"

#include "src/devices/devicemanager.h"
void hueBasicLight::setBridgeId(DeviceId id) {
  _bridgeId = id;
  _bridge =
      std::dynamic_pointer_cast<HueBridge>(getDeviceManager()->getDevice(id));
  if (!_bridge) {
    throw std::runtime_error("Bridge not found");
  }
}

void hueBasicLight::setLightHueId(int64_t id) {
  if (_bridge) {
    auto lights = _bridge->_bridge->lights().getAll();
    for (const auto& light : lights) {
      std::cout << "Light discovered:" << light.getId() << std::endl;
      if (light.getId() == id) {
        _hueId = id;
        std::cout << "Light found:" << light.getId() << std::endl;
      }
    }
  }
  throw std::runtime_error("Light not found");
}

OnOff::ResultSetState hueBasicLight::setState(State) {
  return ResultSetState::error;
}

OnOff::ResultSwitch hueBasicLight::switchState() { return ResultSwitch::error; }

OnOff::ResultGetState hueBasicLight::getState() {
  return ResultGetState::error;
}
