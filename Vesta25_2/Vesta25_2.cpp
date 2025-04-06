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
  bridge->connect(bridge->getAvailableBridges()[0]);  // Connect to the first
                                                      // available bridge
  auto light = std::dynamic_pointer_cast<hueBasicLight>(
      deviceManager->getDevice(lightId));
  light->setBridgeId(bridgeId);
  light->setLightHueId(1);
}
// HueBridge bridge;
// bridge.setId(0);
// bridge.getAvailableBridges();
// bridge.connect(bridge.getAvailableBridges()[0]);
