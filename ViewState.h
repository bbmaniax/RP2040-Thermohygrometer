// ViewState - Manages the current view mode and display orientation.

#ifndef __APP_VIEW_STATE_H__
#define __APP_VIEW_STATE_H__

#include <Arduino.h>

class ViewState {
public:
  ViewState();

public:
  typedef enum {
    VIEW_MODE_ALL = 0,
    VIEW_MODE_TEMPERATURE,
    VIEW_MODE_HUMIDITY,
    VIEW_MODE_PRESSURE,
    VIEW_MODE_COUNT
  } ViewMode;

public:
  void begin();
  ViewMode getViewMode() const;
  void switchToNextViewMode();
  bool isDisplayFlipped() const;
  void flipDisplay();

private:
  ViewMode viewMode;
  bool displayFlipped;
};

#endif  // __APP_VIEW_STATE_H__
