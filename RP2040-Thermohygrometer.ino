// RP2040-Thermohygrometer - Main application file for the thermohygrometer project.

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "AppState.h"
#include "Button.h"
#include "DebugSerial.h"
#include "GND.h"
#include "History.h"
#include "SensorManager.h"
#include "View.h"
#include "ViewState.h"

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
Button button1(BUTTON1_INPUT_PIN);
Button button2(BUTTON2_INPUT_PIN);
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
Adafruit_AHTX0 thermometer;
Adafruit_BMP280 barometer;

SensorManager::SensorValues sensorValues;
SensorManager sensorManager(thermometer, barometer);

int16_t temperatureHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t humidityHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t pressureHistoryBuffer[HISTORY_BUFFER_SIZE];
History temperatureHistory(temperatureHistoryBuffer, HISTORY_BUFFER_SIZE);
History humidityHistory(humidityHistoryBuffer, HISTORY_BUFFER_SIZE);
History pressureHistory(pressureHistoryBuffer, HISTORY_BUFFER_SIZE);

AppState appState(SENSOR_READ_INTERVAL_MS);
ViewState viewState;
View view(viewState, display, DISPLAY_WIDTH, DISPLAY_HEIGHT, PLOT_HORIZONTAL_SPACING);

void setup() {
  DEBUG_SERIAL_BEGIN(9600);
  DEBUG_SERIAL_WAIT_FOR();
  DEBUG_SERIAL_PRINTLN();
  DEBUG_SERIAL_PRINTLN("--");
  DEBUG_SERIAL_PRINTLN("Thermohygrometer");

  gnd1.begin();
  button1.begin();
  button2.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS)) { DEBUG_SERIAL_PRINTLN("Failed to initialize display!"); }
  display.display();

  if (!sensorManager.begin()) { DEBUG_SERIAL_PRINTLN("Failed to initialize sensor manager!"); }

  appState.begin();
  viewState.begin();

  if (!sensorManager.readSensorData(&sensorValues)) { DEBUG_SERIAL_PRINTLN("Failed to read sensors!"); }
  temperatureHistory.fill(sensorValues.temperature);
  humidityHistory.fill(sensorValues.humidity);
  pressureHistory.fill(sensorValues.pressure);

  delay(1000);
}

void loop() {
  static bool needUpdate = true;

  button1.update();
  button2.update();

  if (button1.isClicked()) {
    viewState.switchToNextViewMode();
    needUpdate = true;
  }

  if (button2.isClicked()) {
    viewState.flipDisplay();
    view.flipDisplay();
    needUpdate = true;
  }

  if (appState.shouldReadSensorData()) {
    if (!sensorManager.readSensorData(&sensorValues)) {
      DEBUG_SERIAL_PRINTLN("Failed to read sensors!");
      goto EOL;
    }

    temperatureHistory.prepend(sensorValues.temperature);
    humidityHistory.prepend(sensorValues.humidity);
    pressureHistory.prepend(sensorValues.pressure);

    appState.markSensorDataAsRead();
    needUpdate = true;
  }

  if (needUpdate) view.render(temperatureHistory, humidityHistory, pressureHistory);

EOL:
  delay(10);
}
