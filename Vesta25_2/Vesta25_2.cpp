// Vesta25_2.cpp : Ce fichier contient la fonction 'main'. L'exécution du
// programme commence et se termine à cet endroit.
//

#include <iostream>

#include "sqlite_orm.h"
#include "src/devices/devicemanager.h"
#include "src/devices/huebridge/huebridge.h"
#include "src/devices/lights/hue/huebasiclight.h"
using namespace sqlite_orm;

struct User {
  int id = 0;
  std::string firstName;
  std::string lastName;
  int birthDate = 0;
  std::unique_ptr<std::string> imageUrl;
  int typeId = 0;
};

struct UserType {
  int id;
  std::string name;
};

int main() {
  using namespace sqlite_orm;
  auto storage = make_storage(
      "D:\\db.sqlite",
      make_table("users",
                 make_column("id", &User::id, primary_key().autoincrement()),
                 make_column("first_name", &User::firstName),
                 make_column("last_name", &User::lastName),
                 make_column("birth_date", &User::birthDate),
                 make_column("image_url", &User::imageUrl),
                 make_column("type_id", &User::typeId)),
      make_table(
          "user_types",
          make_column("id", &UserType::id, primary_key().autoincrement()),
          make_column("name", &UserType::name,
                      default_value("name_placeholder"))));
  storage.sync_schema();
  std::cout << "open: " << storage.is_opened() << std::endl;
  User user{-1,
            "Jonh",
            "Doe",
            664416000,
            std::make_unique<std::string>("url_to_heaven"),
            3};

  try {
    storage.insert(user);
  } catch (std::system_error e) {
    std::cout << e.what() << std::endl;
  }

  auto users = storage.get_all<User>();
  for (const auto& user : users) {
    std::cout << user.firstName << std::endl;
  }

  auto deviceManager = std::make_shared<DeviceManager>();
  const auto bridgeId = deviceManager->addDevice<HueBridge>();
  const auto lightId = deviceManager->addDevice<hueBasicLight>();

  auto bridge =
      std::dynamic_pointer_cast<HueBridge>(deviceManager->getDevice(bridgeId));
  auto bridgeConfig = bridge->getAvailableBridges()[0];
  nlohmann::json bridgeJsonConfig;
  bridgeJsonConfig["huebridge"]["ip"] = bridgeConfig.ip;
  bridgeJsonConfig["huebridge"]["port"] = bridgeConfig.port;
  bridgeJsonConfig["huebridge"]["mac"] = bridgeConfig.mac;

  auto res = bridge->configure(bridgeJsonConfig);  // Connect to the first
                                                   // available bridge
  std::cout << res.text << std::endl;
  auto light = std::dynamic_pointer_cast<hueBasicLight>(
      deviceManager->getDevice(lightId));
  nlohmann::json lightJsonConfig;
  lightJsonConfig["huebasiclight"]["bridgeId"] = bridgeId;
  lightJsonConfig["huebasiclight"]["lightId"] = 7;
  res = light->configure(lightJsonConfig);
  std::cout << res.text << std::endl;
  while (true) {
    light->setState(OnOff::State::on);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    light->setState(OnOff::State::off);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
