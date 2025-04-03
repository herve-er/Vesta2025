#pragma once
#include "basiclight.h"
#include "src/capabilities/brightness.h"

class DimmableLight : public BasicLight, public Brightness {
  DimmableLight() : BasicLight(), Brightness() {}
};
