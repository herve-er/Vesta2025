// Vesta25_2.cpp : Ce fichier contient la fonction 'main'. L'exécution du
// programme commence et se termine à cet endroit.
//

#include <iostream>

#include "src/devices/devicemanager.h"
#include "src/devices/huebridge/huebridge.h"
#include "src/devices/lights/hue/huebasiclight.h"
int main() {
  auto deviceManager = std::make_shared<DeviceManager>();
  const auto bridgeId = deviceManager->addDevice<HueBridge>();
  const auto lightId = deviceManager->addDevice<hueBasicLight>();

  auto bridge =
      std::dynamic_pointer_cast<HueBridge>(deviceManager->getDevice(bridgeId));
  auto bridgeConfig = bridge->getAvailableBridges()[0];
  nlohmann::json bridgeJsonConfig;
  bridgeJsonConfig["huebridge"]["ip"] = bridgeConfig.ip;
  bridgeJsonConfig["huebridge"]["port"] = bridgeConfig.port;
  bridgeJsonConfig["huebridge"]["mac"] = bridgeConfig.mac;

  auto res = bridge->configure(bridgeJsonConfig);  // Connect to the first
                                                   // available bridge
  std::cout << res.text << std::endl;
  auto light = std::dynamic_pointer_cast<hueBasicLight>(
      deviceManager->getDevice(lightId));
  nlohmann::json lightJsonConfig;
  lightJsonConfig["huebasiclightJson"]["bridgeId"] = bridgeId;
  lightJsonConfig["huebasiclightJson"]["lightId"] = 7;
  light->configure(lightJsonConfig);
  while (true) {
    light->setState(OnOff::State::on);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    light->setState(OnOff::State::off);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
