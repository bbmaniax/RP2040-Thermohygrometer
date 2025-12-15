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

  int16_t getLatestTemperature() const;
  int16_t getLatestHumidity() const;
  int16_t getLatestPressure() const;
  SensorDataHistory& getTemperatureHistory() const;
  SensorDataHistory& getHumidityHistory() const;
  SensorDataHistory& getPressureHistory() const;

 private:
  SensorDataHistory& temperatureHistory;
  SensorDataHistory& humidityHistory;
  SensorDataHistory& pressureHistory;
};

#endif  // MODEL_H
