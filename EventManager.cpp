// EventManager

#include <Arduino.h>

#include "Button.h"
#include "DebugSerial.h"
#include "EventManager.h"
#include "TimeKeeper.h"

EventManager::EventManager(Button& button1, Button& button2, TimeKeeper& timeKeeper1) : button1(button1), button2(button2), timeKeeper1(timeKeeper1) {}

void EventManager::begin() {
  // DEBUG_SERIAL_PRINTLN("Initializing EventManager");
  button1.begin();
  button2.begin();
  timeKeeper1.begin();
}

void EventManager::update() {
  // DEBUG_SERIAL_PRINTLN("Updating EventManager");
  button1.update();
  button2.update();
  timeKeeper1.update();
}

Button* EventManager::getButton(int index) {
  switch (index) {
    case 0: return &button1;
    case 1: return &button2;
    default: return nullptr;
  }
}

TimeKeeper* EventManager::getTimeKeeper(int index) {
  switch (index) {
    case 0: return &timeKeeper1;
    default: return nullptr;
  }
}
