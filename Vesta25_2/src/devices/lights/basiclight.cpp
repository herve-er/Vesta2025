#include "basiclight.h"

#include <thread>
BasicLight::ResultSwitch BasicLight::switchState() {
  ResultSetState setResult = ResultSetState::error;
  switch (getState()) {
    case ResultGetState::isOn:
      setResult = setState(OnOff::State::off);
      break;
    case ResultGetState::isOff:
      setResult = setState(OnOff::State::on);
      break;
    default:
      return ResultSwitch::error;
  }

  int retry = 0;
  do {
    switch (setResult) {
      case ResultSetState::turnedOff:
        return ResultSwitch::turnedOff;
      case ResultSetState::turnedOn:
        return ResultSwitch::turnedOn;
      case ResultSetState::alreadyOn:
        setResult = setState(OnOff::State::off);
        break;
      case ResultSetState::alreadyOff:
        setResult = setState(OnOff::State::on);
        break;
      default:
        return ResultSwitch::error;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } while (retry++ < 3);
}