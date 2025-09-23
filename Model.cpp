// Model

#include <Arduino.h>

#include "History.h"
#include "Model.h"

Model::Model(History& temperatureHistory, History& humidityHistory, History& pressureHistory)
    : temperatureHistory(temperatureHistory), humidityHistory(humidityHistory), pressureHistory(pressureHistory) {}

void Model::begin(int16_t temperature, int16_t humidity, int16_t pressure) {
  temperatureHistory.fill(temperature);
  humidityHistory.fill(humidity);
  pressureHistory.fill(pressure);
}

void Model::update(int16_t temperature, int16_t humidity, int16_t pressure) {
  temperatureHistory.prepend(temperature);
  humidityHistory.prepend(humidity);
  pressureHistory.prepend(pressure);
}

History& Model::getTemperatureHistory() {
  return temperatureHistory;
}

History& Model::getHumidityHistory() {
  return humidityHistory;
}

History& Model::getPressureHistory() {
  return pressureHistory;
}

int16_t Model::getTemperature() {
  return temperatureHistory.getValue(0);
}

int16_t Model::getHumidity() {
  return humidityHistory.getValue(0);
}

int16_t Model::getPressure() {
  return pressureHistory.getValue(0);
}
