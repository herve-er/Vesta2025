#pragma once
#include <set>
#include <string>

class Capability {
 public:
  virtual ~Capability() = default;
  struct Name : public std::string {
    using std::string::string;
  };
  struct Description : public std::string {
    using std::string::string;
  };

  Name getName() const { return _name; }
  Description getDesc() const { return _desc; }

 protected:
  Capability(const Name& capabilityName, const Description& capabilityDesc);

 private:
  Name _name;
  Description _desc;
  static inline std::set<std::string> _capabilities;
};
