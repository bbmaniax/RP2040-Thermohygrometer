// RP2040-Thermohygrometer - Main application file for the thermohygrometer project.

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "Button.h"
#include "DebugSerial.h"
#include "EventManager.h"
#include "GND.h"
#include "History.h"
#include "SensorManager.h"
#include "TimeKeeper.h"
#include "View.h"

#define BUTTON1_GND_PIN 28
#define BUTTON1_INPUT_PIN 26
#define BUTTON2_INPUT_PIN 29

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_I2C_ADDRESS 0x3C

#define HISTORY_BUFFER_SIZE (DISPLAY_WIDTH / (PLOT_HORIZONTAL_SPACING + 1) + PLOT_HORIZONTAL_SPACING)

#define SENSOR_READ_INTERVAL_MS 3000
#define PLOT_HORIZONTAL_SPACING 1

GND gnd1(BUTTON1_GND_PIN);

TimeKeeper timeKeeper1(SENSOR_READ_INTERVAL_MS);
Button button1(BUTTON1_INPUT_PIN);
Button button2(BUTTON2_INPUT_PIN);
EventManager eventManager(timeKeeper1, button1, button2);

Adafruit_AHTX0 thermometer;
Adafruit_BMP280 barometer;
SensorManager sensorManager(thermometer, barometer);

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

SensorManager::SensorValues sensorValues;
int16_t temperatureHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t humidityHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t pressureHistoryBuffer[HISTORY_BUFFER_SIZE];
History temperatureHistory(temperatureHistoryBuffer, HISTORY_BUFFER_SIZE);
History humidityHistory(humidityHistoryBuffer, HISTORY_BUFFER_SIZE);
History pressureHistory(pressureHistoryBuffer, HISTORY_BUFFER_SIZE);

View view(display, DISPLAY_WIDTH, DISPLAY_HEIGHT, PLOT_HORIZONTAL_SPACING);

void setup() {
  DEBUG_SERIAL_BEGIN(9600);
  DEBUG_SERIAL_WAIT_FOR();
  DEBUG_SERIAL_PRINTLN();
  DEBUG_SERIAL_PRINTLN("--");
  DEBUG_SERIAL_PRINTLN("Thermohygrometer");

  gnd1.begin();

  if (!view.begin()) { DEBUG_SERIAL_PRINTLN("Failed to initialize view!"); }
  if (!eventManager.begin()) { DEBUG_SERIAL_PRINTLN("Failed to initialize event manager!"); }
  if (!sensorManager.begin()) { DEBUG_SERIAL_PRINTLN("Failed to initialize sensor manager!"); }

  if (!sensorManager.readSensorData(&sensorValues)) { DEBUG_SERIAL_PRINTLN("Failed to read sensors!"); }
  temperatureHistory.fill(sensorValues.temperature);
  humidityHistory.fill(sensorValues.humidity);
  pressureHistory.fill(sensorValues.pressure);

  delay(1000);
}

void loop() {
  static bool needUpdate = true;

  if (!eventManager.update()) { DEBUG_SERIAL_PRINTLN("Failed to update event manager!"); }

  if (eventManager.getTimeKeeper(0)->isTimeUp()) {
    if (!sensorManager.readSensorData(&sensorValues)) { DEBUG_SERIAL_PRINTLN("Failed to read sensors!"); }
    temperatureHistory.prepend(sensorValues.temperature);
    humidityHistory.prepend(sensorValues.humidity);
    pressureHistory.prepend(sensorValues.pressure);
    eventManager.getTimeKeeper(0)->reset();
    needUpdate = true;
  }

  if (eventManager.getButton(0)->isClicked()) {
    view.switchToNextViewMode();
    needUpdate = true;
  }

  if (eventManager.getButton(1)->isClicked()) {
    view.flip();
    needUpdate = true;
  }

  if (needUpdate) view.render(temperatureHistory, humidityHistory, pressureHistory);

  delay(10);
}
