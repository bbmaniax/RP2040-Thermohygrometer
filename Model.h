// Model.h - Model for Thermohygrometer

#pragma once

#ifndef MODEL_H
#  define MODEL_H

#include <Arduino.h>

class SensorDataHistory;
class SensorManager;

class Model {
public:
  Model(SensorDataHistory& temperatureHistory, SensorDataHistory& humidityHistory, SensorDataHistory& pressureHistory);

  void begin(int16_t temperature, int16_t humidity, int16_t pressure);
  void update(int16_t temperature, int16_t humidity, int16_t pressure);

  SensorDataHistory& getTemperatureHistory();
  SensorDataHistory& getHumidityHistory();
  SensorDataHistory& getPressureHistory();

  int16_t getLatestTemperature();
  int16_t getLatestHumidity();
  int16_t getLatestPressure();

private:
  SensorDataHistory& temperatureHistory;
  SensorDataHistory& humidityHistory;
  SensorDataHistory& pressureHistory;
};

#endif  // MODEL_H
