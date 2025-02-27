#pragma once

#include <iaprovider/base/iafunction/iafunction.h>
#include <iaprovider/base/iafunction/iafunctioncall.h>
#include <iaprovider/base/message.h>
#include <iaprovider/base/types.h>

namespace vesta {
using responsCallBack = std::function<void(const messagePtr<Message> &message)>;

class AbstractIAProvider {
 public:
  void setResponseCallback(const responsCallBack &callback) {
    _responseCallback = callback;
  };
  void sendMessage(const messagePtr<Message> &message) {
    handleUserMessage(message);
  };

  const std::list<messagePtr<Message>> &getMessagesHistory() {
    return _messagesHistory;
  };

  const void addIAFunction(
      const std::shared_ptr<IAFunctionAbstract<SimpleReturnType>>
          &functionPtr) {
    _functions.emplace_back(functionPtr);
  }

 protected:
  void addMessageToHistory(const messagePtr<Message> &message) {
    // message->print();
    _messagesHistory.emplace_back(message);
  };
  void _sendResponse(const messagePtr<Message> &message) {
    if (_responseCallback) _responseCallback(message);
  };
  const std::vector<std::shared_ptr<IAFunctionAbstract<SimpleReturnType>>> &

  getFunctions() {
    return _functions;
  }
  SimpleReturnType runFunction(const IAFunctionCall &functionCall) {
    for (const auto &function : _functions) {
      if (function->name() == functionCall.functionName()) {
        return function->run(functionCall.args());
      }
    }
    return SimpleReturnType(false,
                            "Error executing function, function not found");
  }

  virtual void handleUserMessage(const messagePtr<Message> &message) = 0;

 private:
  std::vector<std::shared_ptr<IAFunctionAbstract<SimpleReturnType>>> _functions;
  std::list<messagePtr<Message>> _messagesHistory;
  responsCallBack _responseCallback;
};
}  // namespace vesta