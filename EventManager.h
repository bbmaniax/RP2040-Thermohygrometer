// EventManager - Manages input events from buttons

#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

class Button;

class EventManager {
public:
  EventManager(Button& button1, Button& button2);

  typedef void (*ButtonClickHandler)();

  bool begin();
  bool update();

  Button* getButton(int index);

private:
  Button& button1;
  Button& button2;
};

#endif  // __EVENT_MANAGER_H__
