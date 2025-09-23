// AbstractSensor

#ifndef __ABSTRACT_SENSOR_H__
#define __ABSTRACT_SENSOR_H__

#include <Arduino.h>

class AbstractThermometer {
public:
    virtual ~AbstractThermometer() = default;

    virtual void update() = 0;
    virtual int16_t getValue() const = 0;
};

#endif // __ABSTRACT_SENSOR_H__
