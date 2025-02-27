#pragma once
#include <vector>

#include "iaprovider/base/iafunction/iafunction.h"
#include "utility/types.h"

namespace vesta {

class AbstractToolset {
 public:
  AbstractToolset(const std::string& name) : _name(name) { setUp(); }

  const std::map<AbstractToolset,
                 std::vector<std::pair<IAFunction<SimpleReturnType>, int64_t>>>
  searchFunction(const searchRequest& request) const {
    std::map<AbstractToolset,
             std::vector<std::pair<IAFunction<SimpleReturnType>, int64_t>>>
        scoreMap;
    const auto toolsets = getSubToolsetsRecursively();
    for (const auto& subToolset : _subToolsets) {
      auto functions = subToolset->getFunctions();
      for (const auto& function : functions) {
        int64_t score = function->getSearchScore(request);
        scoreMap[subToolset].emplace_back(function, score);
      }
    }
    // TODO: sort the scoreMap and filter.
    return scoreMap;
  }

  const std::vector<sptr<AbstractToolset>>& getSubToolsets() const {
    return _subToolsets;
  }
  std::vector<const AbstractToolset&> getSubToolsetsRecursively() const {
    std::vector<const AbstractToolset&> result;
    result.emplace_back(*this);
    for (const auto& subToolset : _subToolsets) {
      result.emplace_back(subToolset);
      auto subSubToolsets = subToolset->getSubToolsetsRecursively();
      result.insert(result.end(), subSubToolsets.begin(), subSubToolsets.end());
    }
    return result;
  }
  const std::vector<sptr<IAFunction<SimpleReturnType>>>& getFunctions() const {
    return _functions;
  }
  const std::string& getName() const { return _name; }
  const std::string& getFullName() const {
    if (_parentToolset == nullptr) {
      return _name;
    }
    return _parentToolset->getFullName() + "/" + _name;
  }

 protected:
  // Add the subtoolset and function to the toolset
  virtual void setUp() = 0;
  void addSubToolset(AbstractToolset subToolset) {
    for (const auto& existingSubToolset : _subToolsets) {
      if (existingSubToolset->getName() == subToolset.getName()) {
        std::cout << "Subtoolset with the same name already exists ("
                  << subToolset.getName() << ")" << std::endl;
        return;
      }
    }
    _subToolsets.emplace_back(subToolset);
  }
  void addFunction(sptr<IAFunction<SimpleReturnType>> function) {
    _functions.emplace_back(function);
  }

 private:
  std::vector<IAFunction<SimpleReturnType>> getFunctionsRecursively() const {
    std::vector<IAFunction<SimpleReturnType>> result;
    for (const auto& subToolset : _subToolsets) {
      auto subFunctions = subToolset->getFunctionsRecursively();
      result.insert(result.end(), subFunctions.begin(), subFunctions.end());
    }
    return result;
  }

 private:
  std::string _name;
  sptr<AbstractToolset> _parentToolset;
  std::vector<sptr<AbstractToolset>> _subToolsets;
  std::vector<sptr<IAFunction<SimpleReturnType>>> _functions;
};

}  // namespace vesta