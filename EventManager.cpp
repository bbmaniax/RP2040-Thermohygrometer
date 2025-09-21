// EventManager - Manages input events from buttons

#include <Arduino.h>

#include "Button.h"
#include "EventManager.h"

EventManager::EventManager(unsigned long sensorReadIntervalMs, Button& button1, Button& button2) : sensorReadIntervalMs(sensorReadIntervalMs), lastMillis(0), needSensorDataRead(false), button1(button1), button2(button2) {}

bool EventManager::begin() {
  lastMillis = 0;
  needSensorDataRead = false;
  if (!button1.begin()) { return false; }
  if (!button2.begin()) { return false; }
  return true;
}

bool EventManager::update() {
  unsigned long currentMillis = millis();
  needSensorDataRead = (lastMillis == 0 || currentMillis - lastMillis >= sensorReadIntervalMs);

  if (!button1.update()) { return false; }
  if (!button2.update()) { return false; }
  return true;
}

bool EventManager::shouldReadSensorData() {
  return needSensorDataRead;
}

void EventManager::markSensorDataAsRead() {
  lastMillis = millis();
  needSensorDataRead = false;
}

Button* EventManager::getButton(int index) {
  switch (index) {
    case 0: return &button1;
    case 1: return &button2;
    default: return nullptr;
  }
}
