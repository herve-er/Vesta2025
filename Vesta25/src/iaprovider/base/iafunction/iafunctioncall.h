#pragma once
#include <iaprovider/base/iafunction/iafunction.h>
#include <iaprovider/base/types.h>

namespace vesta {
class IAFunctionCall {
 public:
  explicit IAFunctionCall(std::string functionName, json args)
      : _functionName(functionName), _args(args) {}
  explicit IAFunctionCall(std::string functionName, std::string result)
      : _functionName(functionName), _result(result) {}

  std::string functionName() const { return _functionName; }
  std::string result() const { return _result; }
  json args() const { return _args; }

 private:
  std::string _functionName;
  /* Must be a json object with the following structure:
   * {
   * "arg1name": value1,
   * "arg2name": value2,
   * "arg3name": value3,
   * ...
   * }
   */
  json _args = {};
  std::string _result;
};
}  // namespace vesta