#pragma once
#include <iaprovider/base/abstractiaprovider.h>
#include <iaprovider/openai/openaimessage.h>

#include <openai/openai.hpp>

namespace vesta {

class openAIProvider : public AbstractIAProvider {
 public:
  openAIProvider(std::string apiKey) : openai_(apiKey) {}

  void handleUserMessage(const messagePtr<Message>& message) override;

 private:
  ::openai::OpenAI openai_;
  void processMessages();
  std::list<messagePtr<OpenAIMessage>> getOpenAIMessagesHistory();

  messagePtr<OpenAIMessage> rawCompletionToOpenAIMessage(json message);
  std::pair<messagePtr<OpenAIMessage>, messagePtr<OpenAIMessage>>
  buildFunctionCallRespons(
      const std::string& functionName, const SimpleReturnType& result,
      const functionCallId& callId);

  std::string argTypeToString(ArgType argType);
  json convertMessageToOpenAIMessage(messagePtr<Message> message);
  json generateToolsJson();
  json generateToolJson(
      const std::shared_ptr<IAFunctionAbstract<SimpleReturnType>>& function);

  std::string imageToBase64(const std::filesystem::path& imgPath);
};
}  // namespace vesta