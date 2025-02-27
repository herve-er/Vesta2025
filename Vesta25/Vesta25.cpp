
#include <iaprovider/base/iafunction/iafunction.h>
#include <iaprovider/openai/openiaprovider.h>

#include <iostream>

constexpr char apiKey[] =
    "sk-proj-4FuBbVICvLI-xufGTGoomv1gakAhXL5waqjg27SKzT0LyMXAPiDtowMv3oM2N_"
    "4L9u3XRSdb9jT3BlbkFJdITmYKl5lZvxTn3D7Zwsfg_"
    "oCIP36IC6uT0qRrh4bqMMFuWOG6D02OWWsHI2f28_dCL93JvNgA";

#include <computerInteraction\computerinteraction.h>

int main() {
  using namespace vesta;
  // Set mouse position
  std::shared_ptr<IAFunction<SimpleReturnType, int /*x*/, int /*y*/>>
      setMousePositionPercent =
          std::make_shared<IAFunction<SimpleReturnType, int, int>>(
              [](int x, int y) {
                ScreenController::setMousePositionPercent(x, y);
                return SimpleReturnType(true, "Mouse position set");
              });
  setMousePositionPercent->setName("setMousePositionPercent");
  setMousePositionPercent->setDescription("Set the mouse position");
  setMousePositionPercent->setArgsDescription(
      {ArgsDescription("x",
                       "X position between 0 and 100%, 0% is left, 100% "
                       "is right",
                       ArgType::INT),
       ArgsDescription("y",
                       "Y positionbetween 0 and 100%, 0% is top, 100% "
                       "is bottom",
                       ArgType::INT)});

  // Move mouse up
  std::shared_ptr<IAFunction<SimpleReturnType, int /*distance*/>> moveMouseUp =
      std::make_shared<IAFunction<SimpleReturnType, int>>([](int distance) {
        ScreenController::moveMouseUp(distance);
        return SimpleReturnType(true, "Mouse moved up");
      });
  moveMouseUp->setName("moveMouseUp");
  moveMouseUp->setDescription(
      "Move the mouse up, usefull if your mousse is under the place you want "
      "to click");
  moveMouseUp->setArgsDescription(
      {ArgsDescription("distance", "Distance to move", ArgType::INT)});
  // Move mouse down
  std::shared_ptr<IAFunction<SimpleReturnType, int /*distance*/>>
      moveMouseDown =
          std::make_shared<IAFunction<SimpleReturnType, int>>([](int distance) {
            ScreenController::moveMouseDown(distance);
            return SimpleReturnType(true, "Mouse moved down");
          });
  moveMouseDown->setName("moveMouseDown");
  moveMouseDown->setDescription(
      "Move the mouse down, usefull if your mousse is "
      "above the place you want to click");
  moveMouseDown->setArgsDescription(
      {ArgsDescription("distance", "Distance to move", ArgType::INT)});

  // Move mouse left
  std::shared_ptr<IAFunction<SimpleReturnType, int /*distance*/>>
      moveMouseLeft =
          std::make_shared<IAFunction<SimpleReturnType, int>>([](int distance) {
            ScreenController::moveMouseLeft(distance);
            return SimpleReturnType(true, "Mouse moved left");
          });
  moveMouseLeft->setName("moveMouseLeft");
  moveMouseLeft->setDescription(
      "Move the mouse left, usefull if your mousse is "
      "on the right of the place you want to click");
  moveMouseLeft->setArgsDescription(
      {ArgsDescription("distance", "Distance to move", ArgType::INT)});

  // Move mouse right
  std::shared_ptr<IAFunction<SimpleReturnType, int /*distance*/>>
      moveMouseRight =
          std::make_shared<IAFunction<SimpleReturnType, int>>([](int distance) {
            ScreenController::moveMouseRight(distance);
            return SimpleReturnType(true,
                                    "Mouse moved right, usefull if your "
                                    "mousse is on the left of the place "
                                    "you want to click");
          });
  moveMouseRight->setName("moveMouseRight");
  moveMouseRight->setDescription("Move the mouse right");
  moveMouseRight->setArgsDescription(
      {ArgsDescription("distance", "Distance to move", ArgType::INT)});

  // Click
  std::shared_ptr<IAFunction<SimpleReturnType, bool /*left*/>> click =
      std::make_shared<IAFunction<SimpleReturnType, bool>>([](bool left) {
        ScreenController::click(left);
        return SimpleReturnType(true, "Click done");
      });
  click->setName("click");
  click->setDescription("Click on the screen");
  click->setArgsDescription({ArgsDescription(
      "left", "True for left click, false for right click", ArgType::BOOL)});

  // Double click
  std::shared_ptr<IAFunction<SimpleReturnType, bool /*left*/>> doubleClick =
      std::make_shared<IAFunction<SimpleReturnType, bool>>([](bool left) {
        ScreenController::doubleClick(left);
        return SimpleReturnType(true, "Double click done");
      });
  doubleClick->setName("doubleClick");
  doubleClick->setDescription("Double click on the screen");
  doubleClick->setArgsDescription({ArgsDescription(
      "left", "True for left click, false for right click", ArgType::BOOL)});

  // Hold click
  std::shared_ptr<IAFunction<SimpleReturnType, bool /*left*/>> holdClick =
      std::make_shared<IAFunction<SimpleReturnType, bool>>([](bool left) {
        ScreenController::holdClick(left);
        return SimpleReturnType(true, "Click holded");
      });
  holdClick->setName("holdClick");
  holdClick->setDescription(
      "Hold click on the screen until releaseClick, click or doubleClick is "
      "called");
  holdClick->setArgsDescription({ArgsDescription(
      "left", "True for left click, false for right click", ArgType::BOOL)});

  // Release click
  std::shared_ptr<IAFunction<SimpleReturnType, bool /*left*/>> releaseClick =
      std::make_shared<IAFunction<SimpleReturnType, bool>>([](bool left) {
        ScreenController::releaseClick(left);
        return SimpleReturnType(true, "Click released");
      });
  releaseClick->setName("releaseClick");
  releaseClick->setDescription("Release click on the screen");
  releaseClick->setArgsDescription({ArgsDescription(
      "left", "True for left click, false for right click", ArgType::BOOL)});

  // Write
  std::shared_ptr<IAFunction<SimpleReturnType, std::string /*text*/>> write =
      std::make_shared<IAFunction<SimpleReturnType, std::string>>(
          [](const std::string &text) {
            std::wstring wtext(text.begin(), text.end());
            ScreenController::write(wtext);
            return SimpleReturnType(true, "Text written");
          });
  write->setName("write");
  write->setDescription("Write text on the screen");
  write->setArgsDescription(
      {ArgsDescription("text", "Text to write", ArgType::STRING)});

  // Enter
  std::shared_ptr<IAFunction<SimpleReturnType>> enter =
      std::make_shared<IAFunction<SimpleReturnType>>([]() {
        ScreenController::enter();
        return SimpleReturnType(true, "Enter pressed");
      });
  enter->setName("enter");
  enter->setDescription("Press enter key");

  // Escape
  std::shared_ptr<IAFunction<SimpleReturnType>> escape =
      std::make_shared<IAFunction<SimpleReturnType>>([]() {
        ScreenController::escape();
        return SimpleReturnType(true, "Escape pressed");
      });
  escape->setName("escape");
  escape->setDescription("Press escape key");

  // Windows
  std::shared_ptr<IAFunction<SimpleReturnType>> windows =
      std::make_shared<IAFunction<SimpleReturnType>>([]() {
        ScreenController::windows();
        return SimpleReturnType(true, "Windows key pressed");
      });
  windows->setName("windows");
  windows->setDescription("Press windows key");

  // ScreenShot
  std::shared_ptr<IAFunction<SimpleReturnType>> screenShot =
      std::make_shared<IAFunction<SimpleReturnType>>([]() {
        auto imgPath = ScreenController::getScreenShot();
        auto [x, y] = ScreenController::getMousePosition();
        return SimpleReturnType(
            true,
            "Screen shot taken, current mouse position: " + std::to_string(x) +
                ", " + std::to_string(y),
            imgPath);
      });

  screenShot->setName("getscreenShot");
  screenShot->setDescription(
      "Take a screen shot, you will be able to see the result.");

  openAIProvider openAIApi(apiKey);
  openAIApi.setResponseCallback(
      [](const messagePtr<Message> &message) {
        std::cout << "Message received: " << message->text << std::endl;
      });

  openAIApi.addIAFunction(setMousePositionPercent);
  openAIApi.addIAFunction(moveMouseUp);
  openAIApi.addIAFunction(moveMouseDown);
  openAIApi.addIAFunction(moveMouseLeft);
  openAIApi.addIAFunction(moveMouseRight);
  openAIApi.addIAFunction(click);
  openAIApi.addIAFunction(holdClick);
  openAIApi.addIAFunction(releaseClick);
  openAIApi.addIAFunction(write);
  openAIApi.addIAFunction(enter);
  openAIApi.addIAFunction(escape);
  openAIApi.addIAFunction(windows);
  openAIApi.addIAFunction(screenShot);
  
  vesta::messagePtr message =
      std::make_shared<Message>();
  message->role = Role::User;
  message->text =
      "You are an AI assistant designed to autonomously execute tasks on the "
      "user's computer without requiring manual input. You have access to "
      "functions that allow you to control the system, including taking "
      "screenshots to verify progress. Execute all necessary actions in a "
      "single step until you reach a point where you need visual confirmation "
      "via a screenshot. Never ask the user to perform an action—always "
      "execute commands yourself.";
  openAIApi.sendMessage(message);
  while (true) {
    messagePtr message =
        std::make_shared<Message>();
    message->role = Role::User;
    std::getline(std::cin, message->text);

    openAIApi.sendMessage(message);
  }
}