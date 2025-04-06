#include "src/devices/devicemanager.h"

#include <set>
std::shared_ptr<Device> DeviceManager::getDevice(DeviceId id) {
  for (const auto& device : _devices) {
    if (device->getId() == id) {
      return device;
    }
  }
  return nullptr;
}

DeviceId DeviceManager::getNextId() const {
  DeviceId res = 0;
  std::set<DeviceId> ids;
  for (const auto& device : _devices) {
    ids.insert(device->getId());
  }
  DeviceId previousId = 0;
  for (const auto& id : ids) {
    if (id != previousId + 1) break;
    previousId = id;
  }
  res = previousId + 1;

  return res;
}
