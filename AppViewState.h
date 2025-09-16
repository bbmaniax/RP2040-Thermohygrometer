/*
 AppViewState Management Library

 A library for managing the view state, including
 display patterns and orientation.
*/

#ifndef __APP_VIEW_STATE_H__
#define __APP_VIEW_STATE_H__

#include <Arduino.h>

class AppViewState {
public:
  AppViewState();

public:
  void begin();
  uint8_t getPatternIndex() const;
  void switchToNextPattern();
  bool isDisplayFlipped() const;
  void flipDisplay();

private:
  uint8_t patternIndex;
  bool displayFlipped;
};

#endif // __APP_VIEW_STATE_H__
