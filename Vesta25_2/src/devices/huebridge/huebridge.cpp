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

Device::ConfigureResult HueBridge::configure(nlohmann::json cofigurationJson) {
  // Check the JSON validity
  if (!cofigurationJson.contains("huebridge"))
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "huebridge field is required");
  if (!cofigurationJson.at("huebridge").is_object())
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "huebridge should be an object");
  const auto& huebridgeJson = cofigurationJson.at("huebridge");
  if (!huebridgeJson.contains("ip"))
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "ip field is required");
  if (!huebridgeJson.at("ip").is_string())
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "ip field should be string");
  if (!huebridgeJson.contains("mac"))
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "mac field is required");
  if (!huebridgeJson.at("mac").is_string())
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "mac field should be string");
  if (!huebridgeJson.contains("port"))
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "port field is required");
  if (!huebridgeJson.at("port").is_number_integer())
    return ConfigureResult(ConfigureResult::Code::NotEnoughtInformation,
                           "port field should be a number");

  // Convert Json to BridgeIdentification
  BridgeIdentification bridgeIdentification;
  bridgeIdentification.ip = huebridgeJson.at("ip");
  bridgeIdentification.mac = huebridgeJson.at("mac");
  bridgeIdentification.port = huebridgeJson.at("port");

  // Connect to the bridge
  hueplusplus::BridgeFinder finder(std::make_shared<SystemHttpHandler>());
  finder.addUsername("ecb5faad17d8",
                     "hCd8pOMqnK0D9tJ4f1Vp8cBy05Fnvcdf71KCc8WP");
  auto bridge = std::make_unique<hueplusplus::Bridge>(
      finder.getBridge(bridgeIdentification));

  if (bridge == nullptr) {
    return ConfigureResult(ConfigureResult::Code::Failed,
                           "Unable to connect to the bridge. ");
  }
  _bridge.reset(bridge.release());

  return ConfigureResult(ConfigureResult::Code::Success, "Yeaah, it works !");
}
