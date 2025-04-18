#pragma once
#include <cassert>
#include <nlohmann\json.hpp>
#include <set>
#include <string>
using DeviceId = uint64_t;

class DeviceManager;

class Device {
 public:
  Device(std::shared_ptr<DeviceManager> deviceManager)
      : _deviceManager(deviceManager) {}
  virtual ~Device() = default;

  int64_t getId() const { return _id; }
  void setId(int64_t id) {
    assert(_ids.find(id) == _ids.end());
    _ids.insert(id);
    _id = id;
  }

  struct ConfigureResult {
    enum class Code { Success, NotEnoughtInformation, Failed };
    ConfigureResult(Code newCode, std::string newtext)
        : code(newCode), text(newtext) {}
    Code code;
    std::string text;
  };
  virtual ConfigureResult configure(nlohmann::json json) = 0;

 protected:
  std::shared_ptr<DeviceManager> getDeviceManager() const {
    return _deviceManager;
  }

 private:
  inline static std::set<uint64_t> _ids;
  DeviceId _id = 0;

  std::shared_ptr<DeviceManager> _deviceManager;
};