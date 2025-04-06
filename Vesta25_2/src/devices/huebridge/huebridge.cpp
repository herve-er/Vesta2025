#include "huebridge.h"

#include <thread>
std::vector<HueBridge::BridgeIdentification> HueBridge::getAvailableBridges() {
  hueplusplus::BridgeFinder finder(std::make_shared<SystemHttpHandler>());

  std::vector<BridgeIdentification> bridges = finder.findBridges();
  if (bridges.empty()) {
    std::cout << "No bridge found" << std::endl;
    return bridges;
  }
  for (const auto& bridge : bridges) {
    std::cout << "Bridge: " << bridge.mac << " at " << bridge.ip << '\n';
  }
  return bridges;
}

void HueBridge::connect(const BridgeIdentification& bridgeIdentification) {
  hueplusplus::BridgeFinder finder(std::make_shared<SystemHttpHandler>());
  finder.addUsername("ecb5faad17d8",
                     "hCd8pOMqnK0D9tJ4f1Vp8cBy05Fnvcdf71KCc8WP");
  auto bridge = std::make_unique<hueplusplus::Bridge>(
      finder.getBridge(bridgeIdentification));
  _bridge.reset(bridge.release());
}
