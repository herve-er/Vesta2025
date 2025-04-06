#pragma once
#include <memory>
#include <vector>

#include "src/devices/devices.h"

template <typename T>
concept DeviceType = std::derived_from<T, Device>;

class DeviceManager : public std::enable_shared_from_this<DeviceManager> {
 public:
  DeviceManager() = default;
  ~DeviceManager() = default;

  template <DeviceType T>
  DeviceId addDevice();

  std::shared_ptr<Device> getDevice(DeviceId id);

 private:
  DeviceId getNextId() const;
  std::vector<std::shared_ptr<Device>> _devices;
};

template <DeviceType T>
inline DeviceId DeviceManager::addDevice() {
  const auto device = std::make_shared<T>(shared_from_this());
  device->setId(getNextId());
  _devices.push_back(device);
  return device->getId();
}