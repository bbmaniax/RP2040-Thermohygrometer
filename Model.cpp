// Model

#include <Arduino.h>

#include "SensorDataHistory.h"
#include "Model.h"

Model::Model(SensorDataHistory& temperatureHistory, SensorDataHistory& humidityHistory, SensorDataHistory& pressureHistory)
    : temperatureHistory(temperatureHistory), humidityHistory(humidityHistory), pressureHistory(pressureHistory) {}

void Model::begin(int16_t temperature, int16_t humidity, int16_t pressure) {
  // DEBUG_SERIAL_PRINTLN("Initializing Model");
  temperatureHistory.fill(temperature);
  humidityHistory.fill(humidity);
  pressureHistory.fill(pressure);
}

void Model::update(int16_t temperature, int16_t humidity, int16_t pressure) {
  // DEBUG_SERIAL_PRINTLN("Updating Model: T=" + String(temperature / 10.0f, 1) + "C, H=" + String(humidity / 10.0f, 1) + "%, P=" + String(pressure / 10.0f, 1) + "hPa");
  temperatureHistory.prepend(temperature);
  humidityHistory.prepend(humidity);
  pressureHistory.prepend(pressure);
}

SensorDataHistory& Model::getTemperatureHistory() {
  return temperatureHistory;
}

SensorDataHistory& Model::getHumidityHistory() {
  return humidityHistory;
}

SensorDataHistory& Model::getPressureHistory() {
  return pressureHistory;
}

int16_t Model::getLatestTemperature() {
  return temperatureHistory.getValue(0);
}

int16_t Model::getLatestHumidity() {
  return humidityHistory.getValue(0);
}

int16_t Model::getLatestPressure() {
  return pressureHistory.getValue(0);
}
