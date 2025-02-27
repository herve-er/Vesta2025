#pragma once

#include <iaprovider/base/abstractiaprovider.h>
#include <iaprovider/base/types.h>

namespace vesta {
using functionCallId = std::string;
struct OpenAIMessage : public Message {
  using Message::Message;
  json jsonMessage;
  bool newMessage = true;
  std::map<functionCallId, IAFunctionCall&> callIdToFunctionsMap;
  void print() override {
    Message::print();
    std::cout << "Json message: " << jsonMessage.dump(2) << std::endl;
  }
};
};  // namespace vesta