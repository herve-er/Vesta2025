#pragma once
#include "hueplusplus/Bridge.h"
#include "src/devices/devices.h"
#ifdef _MSC_VER
#include "hueplusplus/WinHttpHandler.h"
using SystemHttpHandler = hueplusplus::WinHttpHandler;
#else
#include "hueplusplus/LinHttpHandler.h"
using SystemHttpHandler = hueplusplus::LinHttpHandler;
#endif

class HueBridge : public Device {
 public:
  using BridgeIdentification = hueplusplus::BridgeFinder::BridgeIdentification;
  using Device::Device;
  static std::vector<BridgeIdentification> getAvailableBridges();
  ConfigureResult configure(nlohmann::json cofigurationJson);

  std::shared_ptr<hueplusplus::Bridge> getBridge() { return _bridge; }

 private:
  std::shared_ptr<hueplusplus::Bridge> _bridge;
};
