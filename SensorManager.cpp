// SensorManager.cpp - Temperature and pressure sensor manager

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Arduino.h>

#include "SensorManager.h"

SensorManager::SensorManager(Adafruit_AHTX0& thermometer, Adafruit_BMP280& barometer, unsigned long intervalMs)
    : thermometer(thermometer), barometer(barometer), state(IDLE), lastReadTime(0), resultReady(false) {
  interval = intervalMs;
  lastData.temperature = INVALID_SENSOR_VALUE;
  lastData.humidity = INVALID_SENSOR_VALUE;
  lastData.pressure = INVALID_SENSOR_VALUE;
}

void SensorManager::begin() {
  // Serial.println("Initializing SensorManager");
  if (!thermometer.begin()) { Serial.println("Failed to initialize AHTX0!"); }
  if (!barometer.begin()) { Serial.println("Failed to initialize BMP280!"); }
  state = IDLE;
  lastReadTime = millis() - interval;
  lastData.temperature = INVALID_SENSOR_VALUE;
  lastData.humidity = INVALID_SENSOR_VALUE;
  lastData.pressure = INVALID_SENSOR_VALUE;
  resultReady = false;
}

void SensorManager::update() {
  switch (state) {
    case IDLE:
      if (millis() - lastReadTime >= interval) {
        state = READING;
      }
      break;

    case READING: {
      // Serial.println("Reading sensor data");
      sensors_event_t temperatureEvent, humidityEvent;
      if (thermometer.getEvent(&humidityEvent, &temperatureEvent)) {
        lastData.temperature = static_cast<int16_t>(temperatureEvent.temperature * 10.0f);
        lastData.humidity = static_cast<int16_t>(humidityEvent.relative_humidity * 10.0f);
      } else {
        Serial.println("Failed to read AHTX0!");
      }
      float pressure = barometer.readPressure();
      if (!isnan(pressure)) {
        lastData.pressure = static_cast<int16_t>(pressure / 10.0f);
      } else {
        Serial.println("Failed to read BMP280!");
      }
      resultReady = true;
      lastReadTime = millis();
      state = IDLE;
    } break;
  }
}

bool SensorManager::isReady() const {
  return resultReady;
}

bool SensorManager::consumeReady() {
  bool ready = resultReady;
  if (ready) {
    resultReady = false;
  }
  return ready;
}

SensorManager::SensorData SensorManager::getSensorData() const {
  return lastData;
}
