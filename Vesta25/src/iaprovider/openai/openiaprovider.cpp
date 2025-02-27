#include <iaprovider/base/iafunction/iafunctioncall.h>
#include <iaprovider/openai/openiaprovider.h>

#include <fstream>

using namespace vesta;
using namespace vesta;
using namespace vesta;
namespace vesta {

void openAIProvider::handleUserMessage(const messagePtr<Message>& message) {
  messagePtr<OpenAIMessage> openaiMessage =
      std::make_shared<OpenAIMessage>(message);
  openaiMessage->jsonMessage = convertMessageToOpenAIMessage(message);
  addMessageToHistory(openaiMessage);
  processMessages();
}

void openAIProvider::processMessages() {
  // json request = {{"model", "gpt-4o"}, {"max_tokens", 16384}};
  json request = {{"model", "o1"}, {"max_completion_tokens", 16384}};
  for (const auto& openAimessage : getOpenAIMessagesHistory()) {
    if (!openAimessage->newMessage) {
      json& jsonMessage = openAimessage->jsonMessage;
      if (jsonMessage.contains("content") &&
          jsonMessage["content"].is_array()) {
        jsonMessage["content"].erase(
            std::remove_if(jsonMessage["content"].begin(),
                           jsonMessage["content"].end(),
                           [](const json& content) {
                             return content["type"] == "image_url";
                           }),
            jsonMessage["content"].end());
      }
    }
    request["messages"].push_back(openAimessage->jsonMessage);
    openAimessage->newMessage = false;
  }
  request["tools"] = generateToolsJson();

  json completion = openai_.chat.create(request);
  auto response = rawCompletionToOpenAIMessage(completion);
  addMessageToHistory(response);
  if (!response->text.empty()) _sendResponse(response);

  if (!response->callIdToFunctionsMap.empty()) {
    std::vector<messagePtr<OpenAIMessage>> fakeUserMessagesForImage;
    for (const auto& [callId, functionCall] : response->callIdToFunctionsMap) {
      SimpleReturnType rawResult = runFunction(functionCall);
      auto resultMessages = buildFunctionCallRespons(
          functionCall.functionName(), rawResult, callId);
      addMessageToHistory(resultMessages.first);
      std::cout << "Function called: " << functionCall.functionName()
                << std::endl;
      if (resultMessages.second) {
        fakeUserMessagesForImage.push_back(resultMessages.second);
      }
    }

    for (const auto& message : fakeUserMessagesForImage) {
      addMessageToHistory(message);
    }
    processMessages();
  }
}

messagePtr<OpenAIMessage> openAIProvider::rawCompletionToOpenAIMessage(
    json rawOpenAIrespons) {
  auto openaiMessage = std::make_shared<OpenAIMessage>();
  openaiMessage->role = Role::AI;
  json message = rawOpenAIrespons["choices"][0]["message"];
  openaiMessage->jsonMessage = message;

  if (!message["content"].is_null()) {
    openaiMessage->text = message["content"];
  } else {
    openaiMessage->text = "";
  }

  if (message.contains("tool_calls")) {
    json toolCalls = message["tool_calls"];
    for (const auto& toolCall : toolCalls) {
      std::string functionName = toolCall["function"]["name"];
      json args =
          json::parse(toolCall["function"]["arguments"].get<std::string>());

      openaiMessage->callIdToFunctionsMap.emplace(
          toolCall["id"].get<functionCallId>(),
          openaiMessage->functions.emplace_back(functionName, args));
    }
  }

  return openaiMessage;
}

std::pair<messagePtr<OpenAIMessage>, messagePtr<OpenAIMessage>>
openAIProvider::buildFunctionCallRespons(const std::string& functionName,
                                         const SimpleReturnType& result,
                                         const functionCallId& callId) {
  // tool response message
  auto openaiMessage = std::make_shared<OpenAIMessage>();
  openaiMessage->role = Role::System;
  std::string contentStr =
      result.executed() ? result.resultText()
                        : "Error executing the function, check the arguments";
  openaiMessage->text = contentStr;
  openaiMessage->imgPath = result.imgPath();
  openaiMessage->functions.emplace_back(functionName, contentStr);
  openaiMessage->jsonMessage = {{"role", "tool"}, {"tool_call_id", callId}};

  openaiMessage->jsonMessage["content"] = contentStr;

  // Fake user response message to provide image
  if (result.imgPath().empty()) {
    return {openaiMessage, nullptr};
  }

  auto imageMessage = std::make_shared<OpenAIMessage>();
  imageMessage->role = Role::System;
  std::string contentStr2 =
      "This is not really a message from the user. Here is the image "
      "returned by the tool: " +
      functionName;
  imageMessage->text = contentStr2;
  imageMessage->imgPath = result.imgPath();
  imageMessage->functions.emplace_back(functionName, contentStr2);
  imageMessage->jsonMessage = {{"role", "user"}};

  std::string base64Img = imageToBase64(result.imgPath());
  imageMessage->jsonMessage["content"].push_back(
      {{"type", "text"}, {"text", contentStr2}});
  json imageUrl;
  imageUrl["url"] = "data:image/jpeg;base64," + base64Img;
  imageMessage->jsonMessage["content"].push_back(
      {{"type", "image_url"}, {"image_url", imageUrl}});

  return {openaiMessage, imageMessage};
}

std::list<messagePtr<OpenAIMessage>>
openAIProvider::getOpenAIMessagesHistory() {
  std::list<messagePtr<OpenAIMessage>> messages;
  for (const auto message : getMessagesHistory()) {
    messages.emplace_back(std::static_pointer_cast<OpenAIMessage>(message));
  }
  return messages;
}

std::string openAIProvider::argTypeToString(ArgType argType) {
  switch (argType) {
    case ArgType::STRING:
      return "string";
    case ArgType::INT:
      return "integer";
    case ArgType::FLOAT:
      return "float";
    case ArgType::BOOL:
      return "boolean";
  }
  return "";
}

json openAIProvider::convertMessageToOpenAIMessage(
    messagePtr<Message> message) {
  json openaiMessage;
  switch (message->role) {
    case Role::AI:
      openaiMessage["role"] = "assistant";
      break;
    case Role::User:
      openaiMessage["role"] = "user";
      break;
    case Role::System:
      openaiMessage["role"] = "user";
      break;
    default:
      std::cout << "Unknown role" << std::endl;
  }
  json messageContent;
  messageContent["type"] = "text";
  messageContent["text"] = message->text;
  openaiMessage["content"].push_back(messageContent);
  return openaiMessage;
}

json openAIProvider::generateToolsJson() {
  json toolsJson = json::array();
  for (const auto& function : getFunctions()) {
    toolsJson.push_back(generateToolJson(function));
  }

  return toolsJson;
}

json openAIProvider::generateToolJson(
    const std::shared_ptr<IAFunctionAbstract<SimpleReturnType>>& function) {
  json toolsJson;
  toolsJson["type"] = "function";
  toolsJson["function"] = json::object();
  toolsJson["function"]["name"] = function->name();
  toolsJson["function"]["description"] = function->description();
  toolsJson["function"]["parameters"] = json::object();
  toolsJson["function"]["parameters"]["type"] = "object";
  toolsJson["function"]["parameters"]["properties"] = json::object();
  for (const auto& arg : function->argsDescription()) {
    json argJson;
    argJson["type"] = argTypeToString(arg.type);
    argJson["description"] = arg.description;
    toolsJson["function"]["parameters"]["properties"][arg.name] = argJson;
  }
  return toolsJson;
}

std::string openAIProvider::imageToBase64(const std::filesystem::path& filePath) {
  std::ifstream file(filePath, std::ios::binary);
  if (!file) {
    return "";
  }

  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
  file.close();

  static const char base64_chars[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string base64;
  int val = 0, valb = -6;
  for (unsigned char c : buffer) {
    val = (val << 8) + c;
    valb += 8;
    while (valb >= 0) {
      base64.push_back(base64_chars[(val >> valb) & 0x3F]);
      valb -= 6;
    }
  }
  if (valb > -6)
    base64.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
  while (base64.size() % 4) base64.push_back('=');

  return base64;
}

}  // namespace vesta