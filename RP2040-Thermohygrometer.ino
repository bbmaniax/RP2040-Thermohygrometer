// RP2040-Thermohygrometer - Main application file for the thermohygrometer project.

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "Button.h"
#include "DebugSerial.h"
#include "EventManager.h"
#include "GND.h"
#include "History.h"
#include "Model.h"
#include "SensorManager.h"
#include "TimeKeeper.h"
#include "View.h"

#define BUTTON1_GND_PIN 28

#define BUTTON1_INPUT_PIN 26
#define BUTTON2_INPUT_PIN 29

#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define HISTORY_BUFFER_SIZE (DISPLAY_WIDTH / (PLOT_HORIZONTAL_SPACING + 1) + PLOT_HORIZONTAL_SPACING)

#define SENSOR_READ_INTERVAL_MS 3000
#define PLOT_HORIZONTAL_SPACING 1

GND gnd1(BUTTON1_GND_PIN);
Button button1(BUTTON1_INPUT_PIN);
Button button2(BUTTON2_INPUT_PIN);
Adafruit_AHTX0 thermometer;
Adafruit_BMP280 barometer;
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
TimeKeeper timeKeeper1(SENSOR_READ_INTERVAL_MS);

SensorManager::SensorData sensorData;
int16_t temperatureHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t humidityHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t pressureHistoryBuffer[HISTORY_BUFFER_SIZE];
History temperatureHistory(temperatureHistoryBuffer, HISTORY_BUFFER_SIZE);
History humidityHistory(humidityHistoryBuffer, HISTORY_BUFFER_SIZE);
History pressureHistory(pressureHistoryBuffer, HISTORY_BUFFER_SIZE);

EventManager eventManager(button1, button2, timeKeeper1);
SensorManager sensorManager(thermometer, barometer);

Model model(temperatureHistory, humidityHistory, pressureHistory);
View view(model, display, DISPLAY_WIDTH, DISPLAY_HEIGHT);

void setup() {
  DEBUG_SERIAL_BEGIN(9600);
  DEBUG_SERIAL_WAIT_FOR();
  DEBUG_SERIAL_PRINTLN();
  DEBUG_SERIAL_PRINTLN("--");
  DEBUG_SERIAL_PRINTLN("Thermohygrometer");

  gnd1.begin();

  eventManager.begin();
  sensorManager.begin();

  sensorManager.readSensorData(&sensorData);
  model.begin(sensorData.temperature, sensorData.humidity, sensorData.pressure);

  view.begin(DISPLAY_I2C_ADDRESS);
  delay(1000);

  if (digitalRead(BUTTON2_INPUT_PIN) == LOW) {
    scan(Wire, display);
    while (digitalRead(BUTTON2_INPUT_PIN) == LOW) {}
  }
}

void loop() {
  eventManager.update();

  static bool needUpdate = true;

  if (eventManager.getTimeKeeper(0)->isTimeUp()) {
    // DEBUG_SERIAL_PRINTLN("Time to read sensors");
    sensorManager.readSensorData(&sensorData);
    model.update(sensorData.temperature, sensorData.humidity, sensorData.pressure);
    eventManager.getTimeKeeper(0)->reset();
    needUpdate = true;
  }

  if (eventManager.getButton(0)->isClicked()) {
    DEBUG_SERIAL_PRINTLN("Button 1 clicked");
    view.switchToNextViewMode();
    needUpdate = true;
  }

  if (eventManager.getButton(1)->isClicked()) {
    DEBUG_SERIAL_PRINTLN("Button 2 clicked");
    view.flip();
    needUpdate = true;
  }

  if (needUpdate) {
    // DEBUG_SERIAL_PRINTLN("Time to render views");
    view.render();
    needUpdate = false;
  }

  delay(10);
}

void scan(TwoWire& wire, Adafruit_SSD1306& display) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("SCANNING");
  display.display();
  display.setCursor(0, 16);
  DEBUG_SERIAL_PRINTLN("SCANNING");

  uint8_t deviceCount = 0;
  for (uint8_t address = 1; address < 127; address++) {
    wire.beginTransmission(address);
    uint8_t error = wire.endTransmission();
    if (error == 0) {
      if (deviceCount == 0) {
        display.clearDisplay();
        display.setCursor(0, 0);
      }
      display.printf("%X ", address);
      display.display();
      deviceCount++;

      if (address < 16) DEBUG_SERIAL_PRINT("0");
      DEBUG_SERIAL_PRINTHEX(address);
      DEBUG_SERIAL_PRINT(" ");
    }
    delay(10);
  }

  if (deviceCount == 0) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("NO DEVICES");
    display.display();
    DEBUG_SERIAL_PRINTLN("NO DEVICES");
  }
}
