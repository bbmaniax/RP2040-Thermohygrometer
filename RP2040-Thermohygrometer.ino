/*
  Thermohygrometer

  A thermohygrometer with OLED display
  for temperature, humidity, and pressure monitoring.
*/

#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "AppState.h"
#include "AppView.h"
#include "AppViewState.h"
#include "Button.h"
#include "DebugSerial.h"
#include "GND.h"
#include "History.h"
#include "SensorManager.h"

#define BUTTON1_GND_PIN 28
#define BUTTON1_INPUT_PIN 26
#define BUTTON2_INPUT_PIN 29

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_I2C_ADDRESS 0x3C

#define DISPLAY_UPDATE_INTERVAL_MS 3000

GND gnd1(BUTTON1_GND_PIN);
Button button1(BUTTON1_INPUT_PIN);
Button button2(BUTTON2_INPUT_PIN);

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

Adafruit_AHTX0 thermometer;
Adafruit_BMP280 barometer;
SensorManager sensorManager(thermometer, barometer);

History temperatureHistory(DISPLAY_WIDTH + 1);
History humidityHistory(DISPLAY_WIDTH + 1);
History pressureHistory(DISPLAY_WIDTH + 1);

AppState appState(DISPLAY_UPDATE_INTERVAL_MS);
AppViewState appViewState;
AppView appView(display, DISPLAY_WIDTH, DISPLAY_HEIGHT);

void setup() {
  SERIAL_BEGIN(115200);
  WAIT_FOR_SERIAL();
  SERIAL_PRINTLN();
  SERIAL_PRINTLN("--");

  gnd1.begin();
  button1.begin();
  button2.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS)) {
    SERIAL_PRINTLN("Failed to initialize display!");
    abend();
  }
  display.display();

  if (!sensorManager.begin()) {
    SERIAL_PRINTLN("Failed to initialize sensor manager!");
    abend();
  }

  appState.begin();
  appViewState.begin();

  float temperature, humidity, pressure;
  if (sensorManager.readSensorData(&temperature, &humidity, &pressure)) {
    temperatureHistory.fill(temperature);
    humidityHistory.fill(humidity);
    pressureHistory.fill(pressure);
  } else {
    SERIAL_PRINTLN("Failed to read sensors!");
  }

  SERIAL_PRINTLN("Thermohygrometer");
  delay(3000);
}

void loop() {
  bool needUpdate = false;

  button1.update();
  button2.update();

  if (button1.isClicked()) {
    appViewState.switchToNextPattern();
    needUpdate = true;
  }

  if (button2.isClicked()) {
    appViewState.flipDisplay();
    display.setRotation(appViewState.isDisplayFlipped() ? 2 : 0);
    needUpdate = true;
  }

  if (appState.shouldReadSensorData()) {
    float temperature, humidity, pressure;
    if (!sensorManager.readSensorData(&temperature, &humidity, &pressure)) {
      SERIAL_PRINTLN("Failed to read sensors!");
      goto EOL;
    }

    temperatureHistory.prepend(temperature);
    humidityHistory.prepend(humidity);
    pressureHistory.prepend(pressure);

    appState.markSensorDataAsRead();
    needUpdate = true;
  }

  if (needUpdate) appView.render(appViewState.getPatternIndex(), temperatureHistory, humidityHistory, pressureHistory);

EOL:
  delay(10);
}

void abend() {
  while (true) { delay(100); }
}
