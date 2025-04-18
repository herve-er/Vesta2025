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
    auto lights = _bridge->getBridge()->lights().getAll();
    for (const auto &light : lights) {
      std::cout << "Light discovered:" << light.getId() << std::endl;
      if (light.getId() == id) {
        _hueId = id;
        std::cout << "Light found:" << light.getId() << std::endl;
      }
    }
  }
  throw std::runtime_error("Light not found");
}

Device::ConfigureResult hueBasicLight::configure(
    nlohmann::json cofigurationJson) {
  // Check the JSON validity
  if (!cofigurationJson.contains("huebasiclight"))
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "huebasiclight field is required");
  if (!cofigurationJson.at("huebasiclight").is_object())
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "huebasiclight should be an object");
  const auto &huebasiclightJson = cofigurationJson.at("huebasiclight");

  if (!huebasiclightJson.contains("bridgeId"))
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "bridgeId field is required");
  if (!huebasiclightJson.at("bridgeId").is_number_integer())
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "bridgeId field should be a number");
  if (!huebasiclightJson.contains("lightId"))
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "lightId field is required");
  if (!huebasiclightJson.at("lightId").is_number_integer())
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "lightId field should be a number");

  // Find the bridge device
  auto hueBridge = getDeviceManager()->getDevice<HueBridge>(
      huebasiclightJson.at("bridgeId"));
  if (!hueBridge) {
    return ConfigureResult(ConfigureResult::Code::Failed,
                           "No hue bridge found with the provided Id");
  }
  auto rawBridge = hueBridge->getBridge();
  try {
    rawBridge->lights().refresh();
    _light = std::make_unique<hueplusplus::Light>(
        rawBridge->lights().get(huebasiclightJson.at("lightId")));
  } catch (std::system_error &sysErr) {
    return ConfigureResult(
        ConfigureResult::Code::Failed,
        "Unexpected error while communicating with the light: " +
            std::string(sysErr.what()));
  } catch (hueplusplus::HueAPIResponseException &hueErr) {
    return ConfigureResult(ConfigureResult::Code::Failed,
                           "Unexpected error while finding the light: " +
                               std::string(hueErr.what()));
  } catch (hueplusplus::HueException &hueErr) {
    return ConfigureResult(ConfigureResult::Code::Failed,
                           "Unexpected error while finding the light: " +
                               std::string(hueErr.what()));
  } catch (nlohmann::json::parse_error &hueErr) {
    return ConfigureResult(ConfigureResult::Code::Failed,
                           "Unexpected error while finding the light: " +
                               std::string(hueErr.what()));
  } catch (...) {
    return ConfigureResult(ConfigureResult::Code::Failed,
                           "Unexpected error while finding the light");
  }

  return ConfigureResult(ConfigureResult::Code::Success, "yeay");
}

OnOff::ResultSetState hueBasicLight::setState(State newState) {
  if (!_light) {
    return ResultSetState::error;
  }
  switch (newState) {
    case State::on:
      if (_light->isOn()) return ResultSetState::alreadyOn;
      _light->on();
      return ResultSetState::turnedOn;
      break;
    case State::off:
      if (!_light->isOn()) return ResultSetState::alreadyOff;
      _light->off();
      return ResultSetState::turnedOff;
      break;
    default:
      return ResultSetState::error;
  }
}

OnOff::ResultGetState hueBasicLight::getState() {
  return ResultGetState::error;
}
