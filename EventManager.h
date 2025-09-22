// EventManager - Manages input events from buttons

#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

class TimeKeeper;
class Button;

class EventManager {
public:
  EventManager(TimeKeeper& timeKeeper1, Button& button1, Button& button2);

  void begin();
  void update();

  TimeKeeper* getTimeKeeper(int index);
  Button* getButton(int index);

private:
  TimeKeeper& timeKeeper1;
  Button& button1;
  Button& button2;
};

#endif  // __EVENT_MANAGER_H__
