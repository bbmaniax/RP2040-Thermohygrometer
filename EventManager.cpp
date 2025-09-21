// EventManager - Manages input events from buttons

#include <Arduino.h>

#include "Button.h"
#include "EventManager.h"

EventManager::EventManager(Button& button1, Button& button2) : button1(button1), button2(button2) {}

bool EventManager::begin() {
  if (!button1.begin()) { return false; }
  if (!button2.begin()) { return false; }
  return true;
}

bool EventManager::update() {
  if (!button1.update()) { return false; }
  if (!button2.update()) { return false; }
  return true;
}

Button* EventManager::getButton(int index) {
  switch (index) {
    case 0: return &button1;
    case 1: return &button2;
    default: return nullptr;
  }
}
