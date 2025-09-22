// EventManager - Manages input events from buttons

#include <Arduino.h>

#include "Button.h"
#include "DebugSerial.h"
#include "EventManager.h"
#include "TimeKeeper.h"

EventManager::EventManager(TimeKeeper& timeKeeper1, Button& button1, Button& button2) : timeKeeper1(timeKeeper1), button1(button1), button2(button2) {}

void EventManager::begin() {
  DEBUG_SERIAL_PRINTLN("Initializing EventManager");
  timeKeeper1.begin();
  button1.begin();
  button2.begin();
}

void EventManager::update() {
  // DEBUG_SERIAL_PRINTLN("Updating EventManager");
  timeKeeper1.update();
  button1.update();
  button2.update();
}

TimeKeeper* EventManager::getTimeKeeper(int index) {
  switch (index) {
    case 0: return &timeKeeper1;
    default: return nullptr;
  }
}

Button* EventManager::getButton(int index) {
  switch (index) {
    case 0: return &button1;
    case 1: return &button2;
    default: return nullptr;
  }
}
