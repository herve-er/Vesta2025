#include "capability.h"

#include <cassert>

Capability::Capability(const Name& capabilityName,
                       const Description& capabilityDesc) {
  assert(_capabilities.contains(capabilityName) == false);
  _capabilities.insert(capabilityName);
  _name = capabilityName;
  _desc = capabilityDesc;
}
