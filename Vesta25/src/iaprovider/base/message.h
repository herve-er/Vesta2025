#pragma once
#include <iaprovider/base/iafunction/iafunction.h>
#include <iaprovider/base/iafunction/iafunctioncall.h>
#include <iaprovider/base/types.h>

#include <memory>

namespace vesta {
struct Message {
  Message() = default;
  Message(const Message &) = default;
  Message(const std::shared_ptr<Message> &message) : Message(*message) {}
  Message(const messageText &text, Role role) : text(text), role(role) {}
  virtual void print() {
    std::cout << "////////////////////////////////" << std::endl;

    std::cout << "Message: " << text << std::endl;
    if (!imgPath.empty()) {
      std::cout << "Image path: " << imgPath << std::endl;
    }
    std::cout << "Role: " << toString(role) << std::endl;
    if (!functions.empty()) {
      for (auto &function : functions) {
        std::cout << "Function: " << function.functionName() << std::endl;
        std::cout << "Args: " << function.args().dump(2) << std::endl;
      }
    }
  }
  messageText text;
  std::filesystem::path imgPath;
  Role role = Role::Unknown;
  std::list<IAFunctionCall> functions;
};

template <class messageType>
concept MessageDerivated = std::is_base_of_v<Message, messageType>;

template <MessageDerivated T>
using messagePtr = std::shared_ptr<T>;

};  // namespace vesta