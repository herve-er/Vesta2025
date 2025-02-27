#pragma once

#include <iaprovider/base/types.h>

#include <cassert>
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>

using json = nlohmann::json;

namespace vesta {
enum class ArgType { STRING, INT, FLOAT, BOOL };
inline std::wstring ToString(ArgType argType) {
  switch (argType) {
    case ArgType::STRING:
      return L"STRING";
    case ArgType::INT:
      return L"INT";
    case ArgType::FLOAT:
      return L"FLOAT";
    case ArgType::BOOL:
      return L"BOOL";
  }
  return L"";
}

template <typename T>
concept allowedParameterType =
    std::is_same_v<T, std::string> || std::is_same_v<T, int> ||
    std::is_same_v<T, float> || std::is_same_v<T, bool>;

template <allowedParameterType T>
bool jsonIsType(const json &obj) {
  if constexpr (std::is_same_v<T, std::string>) {
    return obj.is_string();
  } else if constexpr (std::is_same_v<T, int>) {
    return obj.is_number_integer();
  } else if constexpr (std::is_same_v<T, float>) {
    return obj.is_number_float();
  } else if constexpr (std::is_same_v<T, bool>) {
    return obj.is_boolean();
  }
  return false;
}

struct SimpleReturnType {
  SimpleReturnType() = default;
  SimpleReturnType(bool executed) : _executed(executed) {}
  SimpleReturnType(bool executed, std::string respons)
      : _executed(executed), _respons(respons) {}
  SimpleReturnType(bool executed, std::string respons,
                   std::filesystem::path imgPath)
      : _executed(executed), _respons(respons), _imgPath(imgPath) {}

  bool executed() const { return _executed; }
  std::string resultText() const { return _respons; }
  std::filesystem::path imgPath() const { return _imgPath; }

 private:
  std::string _respons;
  std::filesystem::path _imgPath;
  bool _executed = false;
};

template <typename T>
concept returnAble = std::is_base_of_v<T, SimpleReturnType>;

struct ArgsDescription {
  ArgsDescription() = default;
  ArgsDescription(std::string name, std::string description, ArgType type)
      : name(name), description(description), type(type) {}
  std::string name;
  std::string description;
  ArgType type;
};

template <returnAble returnType>
struct IAFunctionAbstract {
  //* @brief Run the function with the arguments in the json object.
  //* @details This function expects a json array as input. It will run the
  // function with the arguments in the json object. If the Json is not an array
  // or if the number of elements in the array is different from the number of
  // arguments in the function or if the types of the elements in the array are
  // different from the types of the arguments in the function, the function
  // will return false. Otherwise, it will return true.
  returnType run(json jsonObj) {
    json array = json::array();
    for (auto &args : argsDescription()) {
      if (!jsonObj.contains(args.name)) {
        return {false, "Missing argument: " + args.name};
      }
      array.push_back(jsonObj[args.name]);
    }

    if (!array.is_array()) {
      return {false, "Arguments should be in a json array"};
    }

    if (!checkArgsNumbers(array)) {
      return {false, "Wrong number of arguments"};
    }

    if (!checkArgsTypes(array)) {
      return {false, "Wrong types of arguments"};
    }

    return runFunction(array);
  };

  void setName(const std::string &name) { _name = name; }
  void setDescription(const std::string &description) {
    _description = description;
  }
  const std::string &name() const { return _name; }
  const std::string &description() const { return _description; }
  const std::vector<ArgsDescription> &argsDescription() {
    return _argsDescription;
  }

  virtual int64_t getSearchScore(const searchRequest &request) const {
    int64_t score = 0;
    // TODO: implement the function
    return score;
  }

 protected:
  std::vector<ArgsDescription> _argsDescription;

 private:
  std::string _name;
  std::string _description;
  virtual bool checkArgsNumbers(const json &args) = 0;
  virtual bool checkArgsTypes(const json &args) = 0;
  virtual returnType runFunction(const json &args) = 0;
};

template <returnAble returnType, allowedParameterType... Args>
class IAFunction : public IAFunctionAbstract<returnType>,
                   public std::function<returnType(Args...)> {
 public:
  using std::function<returnType(Args...)>::function;

  void setArgsDescription(
      const std::array<ArgsDescription, sizeof...(Args)> &argsDescription) {
    for (const auto &arg : argsDescription) {
      IAFunctionAbstract<returnType>::_argsDescription.push_back(arg);
    }
  }

  const std::vector<ArgsDescription> &getArgsDescription() {
    assert(IAFunctionAbstract<returnType>::_argsDescription.size() ==
               sizeof...(Args) &&
           "The number of arguments description should be equal to the number "
           "of arguments in the function.");
    return IAFunctionAbstract<returnType>::_argsDescription;
  }

 private:
  //* @brief Check if the number of arguments in the json object is correct.
  //* @details This function expects a json array as input. It will return true
  // if the number of elements in the array is equal to the number of arguments
  // in the function. Undefine behavior if the input is not a json array.
  //* @param args The json array containing the arguments.
  //* @return True if the number of arguments is correct, false otherwise.
  bool checkArgsNumbers(const json &jsonObj) override {
    assert(jsonObj.is_array());
    return (jsonObj.size() == size_t(sizeof...(Args)));
  };

  //* @brief Check if the types of the arguments in the json object are correct.
  //* @details This function expects a json array as input. It will return true
  // if the types of the elements in the array are the same as the types of the
  // arguments in the function. Undefine behavior if the input is not a json
  // array or if the number of elements in the array is different from the
  // number of arguments in the function.
  //* @param args The json array containing the arguments.
  //* @return True if the types of the arguments are matching with the types of
  // the arguments in the function, false otherwise.
  bool checkArgsTypes(const json &jsonObj) override {
    assert(jsonObj.is_array());
    int i = 0;
    return (jsonIsType<Args>(jsonObj[i++]) && ...);
  }

  //* @brief Run the function with the arguments in the json object.
  //* @details This function expects a json array as input. It will run the
  // function with the arguments in the json object. Undefine behavior if the
  // input is not a json array or if the number of elements in the array is
  // different from the number of arguments in the function or if the types of
  // the elements in the array are different from the types of the arguments in
  // the function. You should call checkArgsNumbers and checkArgsTypes before
  // calling this function.
  returnType runFunction(const json &jsonObj) override {
    uint64_t i = sizeof...(Args) - 1;
    return this->operator()(jsonObj[i--].get<Args>()...);
  }
};
}  // namespace vesta