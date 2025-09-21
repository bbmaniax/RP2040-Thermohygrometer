// EventManager - Manages input events from buttons

#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

class Button;

class EventManager {
public:
  EventManager(unsigned long sensorReadIntervalMs, Button& button1, Button& button2);

  typedef void (*ButtonClickHandler)();

  bool begin();
  bool update();

  bool shouldReadSensorData();
  void markSensorDataAsRead();

  Button* getButton(int index);

private:
  unsigned long sensorReadIntervalMs;
  unsigned long lastMillis;
  bool needSensorDataRead;

  Button& button1;
  Button& button2;
};

#endif  // __EVENT_MANAGER_H__
