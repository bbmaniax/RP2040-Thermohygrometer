// Model.h - Model for Thermohygrometer

#pragma once

#ifndef MODEL_H
#  define MODEL_H

#  include <Arduino.h>

class SensorDataHistory;
class SensorManager;

class Model {
 public:
  Model(SensorDataHistory& temperatureHistory, SensorDataHistory& humidityHistory, SensorDataHistory& pressureHistory);

  void begin();
  void update(int16_t temperature, int16_t humidity, int16_t pressure);

  int16_t getTemperature() const;
  int16_t getHumidity() const;
  int16_t getPressure() const;
  SensorDataHistory& getTemperatureHistory() const;
  SensorDataHistory& getHumidityHistory() const;
  SensorDataHistory& getPressureHistory() const;

 private:
  SensorDataHistory& temperatureHistory;
  SensorDataHistory& humidityHistory;
  SensorDataHistory& pressureHistory;
};

#endif  // MODEL_H
