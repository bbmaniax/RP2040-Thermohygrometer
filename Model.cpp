// Model.cpp - Model for Thermohygrometer

#include <Arduino.h>

#include "SensorDataHistory.h"
#include "Model.h"

Model::Model(SensorDataHistory& temperatureHistory, SensorDataHistory& humidityHistory, SensorDataHistory& pressureHistory)
    : temperatureHistory(temperatureHistory), humidityHistory(humidityHistory), pressureHistory(pressureHistory) {}

void Model::begin() {
  // DEBUG_SERIAL_PRINTLN("Initializing Model");
  temperatureHistory.begin();
  humidityHistory.begin();
  pressureHistory.begin();
}

void Model::update(int16_t temperature, int16_t humidity, int16_t pressure) {
  // DEBUG_SERIAL_PRINTLN("Updating Model: T=" + String(temperature / 10.0f, 1) + "C, H=" + String(humidity / 10.0f, 1) + "%, P=" + String(pressure / 10.0f, 1) + "hPa");
  temperatureHistory.prepend(temperature);
  humidityHistory.prepend(humidity);
  pressureHistory.prepend(pressure);
}

int16_t Model::getTemperature() const {
  return temperatureHistory.getValue(0);
}

int16_t Model::getHumidity() const {
  return humidityHistory.getValue(0);
}

int16_t Model::getPressure() const {
  return pressureHistory.getValue(0);
}

SensorDataHistory& Model::getTemperatureHistory() const {
  return temperatureHistory;
}

SensorDataHistory& Model::getHumidityHistory() const {
  return humidityHistory;
}

SensorDataHistory& Model::getPressureHistory() const {
  return pressureHistory;
}
