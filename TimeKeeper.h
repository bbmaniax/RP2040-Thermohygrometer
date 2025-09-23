// TimerKeeper

#ifndef __TIME_KEEPER_H__
#define __TIME_KEEPER_H__

#include <Arduino.h>

class TimeKeeper {
public:
  TimeKeeper(unsigned long intervalMs);

  void begin();
  void update();
  void reset();

  bool isTimeUp() const;

private:
  unsigned long intervalMs;
  unsigned long lastMillis;

  bool timeUp;
};

#endif  // __TIME_KEEPER_H__
