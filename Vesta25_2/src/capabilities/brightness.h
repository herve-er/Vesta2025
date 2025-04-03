#pragma once
#include "capability.h"
#include "cassert"

class Brightness : public Capability {
  static constexpr char capabilityName[] = "Brightness";
  static constexpr char capabilityDesc[] =
      "The brightness of the device can be set and queried.";

 protected:
  Brightness() : Capability(capabilityName, capabilityDesc) {}

  enum class ResultCode {
    success,
    error,
  };

  // The brightness level is a value between 0.0 and 1.0.
  class Level {
   public:
    explicit Level(double level) : _level(level) {
      assert(level >= 0.0 && level <= 1.0);
    }
    double getLevel() const { return _level; }

   private:
    double _level = 0.0;
  };

  class GetLevelResult : Level {
   public:
    GetLevelResult(double level, ResultCode code) : Level(level), _code(code) {}
    ResultCode getCode() const { return _code; }

   private:
    ResultCode _code;
  };

  virtual ResultCode setBrightness(const Level& brightness) = 0;
  virtual const GetLevelResult getBrightness() = 0;

 public:
  virtual ~Brightness() = default;
};
