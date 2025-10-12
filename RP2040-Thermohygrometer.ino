// RP2040-Thermohygrometer

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "Button.h"
#include "DebugSerial.h"
#include "History.h"
#include "Model.h"
#include "SensorManager.h"
#include "TimeKeeper.h"
#include "View.h"

#define BUTTON_PIN 29
#define SENSOR_READ_INTERVAL_MS 3000
#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define HISTORY_BUFFER_SIZE (DISPLAY_WIDTH / (PLOT_HORIZONTAL_SPACING + 1) + PLOT_HORIZONTAL_SPACING)
#define PLOT_HORIZONTAL_SPACING 1

Button button(BUTTON_PIN);
TimeKeeper timeKeeper(SENSOR_READ_INTERVAL_MS);
Adafruit_AHTX0 thermometer;
Adafruit_BMP280 barometer;
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

SensorManager sensorManager(thermometer, barometer);

int16_t temperatureHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t humidityHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t pressureHistoryBuffer[HISTORY_BUFFER_SIZE];
History temperatureHistory(temperatureHistoryBuffer, HISTORY_BUFFER_SIZE);
History humidityHistory(humidityHistoryBuffer, HISTORY_BUFFER_SIZE);
History pressureHistory(pressureHistoryBuffer, HISTORY_BUFFER_SIZE);

Model model(temperatureHistory, humidityHistory, pressureHistory);
View view(model,display, DISPLAY_WIDTH, DISPLAY_HEIGHT);

void setup() {
  DEBUG_SERIAL_BEGIN(9600);
  DEBUG_SERIAL_WAIT_FOR();
  DEBUG_SERIAL_PRINTLN();
  DEBUG_SERIAL_PRINTLN("--");
  DEBUG_SERIAL_PRINTLN("Thermohygrometer");

  button.begin();
  timeKeeper.begin();
  sensorManager.begin();

  static SensorManager::SensorData sensorData;
  sensorManager.acquire(&sensorData);

  model.begin(sensorData.temperature, sensorData.humidity, sensorData.pressure);
  view.begin(DISPLAY_I2C_ADDRESS);
  delay(1000);

  if (digitalRead(BUTTON_PIN) == LOW) {
    scan(Wire, display);
    while (digitalRead(BUTTON_PIN) == LOW) {}
  }
}

void loop() {
  button.update();
  timeKeeper.update();

  static bool needRender = true;

  if (timeKeeper.isTimeUp()) {
    // DEBUG_SERIAL_PRINTLN("Time to read sensors");
    SensorManager::SensorData sensorData;
    sensorManager.acquire(&sensorData);
    model.update(sensorData.temperature, sensorData.humidity, sensorData.pressure);
    timeKeeper.reset();
    needRender = true;
    DEBUG_SERIAL_PRINTLN("T:" + String(sensorData.temperature / 10.0f, 1) + " H:" + String(sensorData.humidity / 10.0f, 1) + " P:" + String(sensorData.pressure / 10.0f, 1));
  }

  if (button.isLongPressed()) {
    // DEBUG_SERIAL_PRINTLN("Button 1 long pressed");
    view.flip();
    needRender = true;
  }

  if (button.isClicked()) {
    // DEBUG_SERIAL_PRINTLN("Button 1 clicked");
    view.switchToNextViewMode();
    needRender = true;
  }

  if (needRender) {
    // DEBUG_SERIAL_PRINTLN("Time to render views");
    view.render();
    needRender = false;
  }

  delay(10);
}

void scan(TwoWire& wire, Adafruit_SSD1306& display) {
  DEBUG_SERIAL_PRINTLN("SCANNING");
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("SCANNING");
  display.display();
  display.setCursor(0, 16);

  uint8_t deviceCount = 0;
  for (uint8_t address = 1; address < 127; address++) {
    wire.beginTransmission(address);
    uint8_t error = wire.endTransmission();
    if (error == 0) {
      if (address < 16) DEBUG_SERIAL_PRINT("0");
      DEBUG_SERIAL_PRINTHEX(address);
      DEBUG_SERIAL_PRINT(" ");
      if (deviceCount == 0) {
        display.clearDisplay();
        display.setCursor(0, 0);
      }
      display.printf("%X ", address);
      display.display();
      deviceCount++;
    }
    delay(10);
  }

  if (deviceCount == 0) {
    DEBUG_SERIAL_PRINTLN("NO DEVICES");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("NO DEVICES");
    display.display();
  }
}
