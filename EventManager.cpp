// EventManager - Manages input events from buttons

#include <Arduino.h>

#include "Button.h"
#include "EventManager.h"
#include "TimeKeeper.h"

EventManager::EventManager(TimeKeeper& timeKeeper1, Button& button1, Button& button2) : timeKeeper1(timeKeeper1), button1(button1), button2(button2) {}

bool EventManager::begin() {
  timeKeeper1.begin();
  if (!button1.begin()) { return false; }
  if (!button2.begin()) { return false; }
  return true;
}

bool EventManager::update() {
  timeKeeper1.update();
  if (!button1.update()) { return false; }
  if (!button2.update()) { return false; }
  return true;
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
