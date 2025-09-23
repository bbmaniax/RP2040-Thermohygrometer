// EventManager

#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

class Button;
class TimeKeeper;

class EventManager {
public:
  EventManager(Button& button1, Button& button2, TimeKeeper& timeKeeper1);

  void begin();
  void update();

  Button* getButton(int index);
  TimeKeeper* getTimeKeeper(int index);

private:
  Button& button1;
  Button& button2;
  TimeKeeper& timeKeeper1;
};

#endif  // __EVENT_MANAGER_H__
