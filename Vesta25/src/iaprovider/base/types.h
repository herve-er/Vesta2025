#pragma once

#include <string>

namespace vesta {
enum class Role { Unknown, User, AI, System };
inline std::string toString(Role role) {
  switch (role) {
    case Role::User:
      return "User";
    case Role::AI:
      return "AI";
    case Role::System:
      return "System";
    default:
      return "Unknown";
  }
}

struct searchRequest {};

using messageText = std::string;
}  // namespace vesta