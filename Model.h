// Model

#ifndef __MODEL_H__
#define __MODEL_H__

#include <Arduino.h>

class History;
class SensorManager;

class Model {
public:
  Model(History& temperatureHistory, History& humidityHistory, History& pressureHistory);

  void begin(int16_t temperature, int16_t humidity, int16_t pressure);
  void update(int16_t temperature, int16_t humidity, int16_t pressure);

  History& getTemperatureHistory();
  History& getHumidityHistory();
  History& getPressureHistory();

  int16_t getTemperature();
  int16_t getHumidity();
  int16_t getPressure();

private:
  History& temperatureHistory;
  History& humidityHistory;
  History& pressureHistory;
};

#endif // __MODEL_H__
