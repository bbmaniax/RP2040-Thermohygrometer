/*
 AppViewState Management Library

 A library for managing the view state, including
 display patterns and orientation.
*/

#ifndef APPVIEWSTATE_H
#define APPVIEWSTATE_H

#include <Arduino.h>

class AppViewState {
public:
  AppViewState();

  void begin();
  uint8_t getPatternIndex() const;
  void switchToNextPattern();
  bool isDisplayFlipped() const;
  void flipDisplay();

private:
  uint8_t patternIndex;
  bool displayFlipped;
};

#endif
