/*
 ViewState Management Library

 A library for managing the view state, including
 display patterns and orientation.
*/

#ifndef VIEWSTATE_H
#define VIEWSTATE_H

#include <Arduino.h>

class ViewState {
public:
  ViewState();

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
