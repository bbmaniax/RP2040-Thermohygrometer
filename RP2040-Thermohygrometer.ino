// RP2040-Thermohygrometer

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "DebugSerial.h"
#include "DigitalButton.h"
#include "Model.h"
#include "SensorDataHistory.h"
#include "SensorManager.h"
#include "View.h"

#define SERIAL_SPEED 115200
#define RGBLED_PIN 16
#define RGBLED_BRIGHTNESS 127
#define BUTTON_PIN 29
#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define PLOT_HORIZONTAL_SPACING 1
#define HISTORY_BUFFER_SIZE ((DISPLAY_WIDTH / PLOT_HORIZONTAL_SPACING) + 1)
#define SENSOR_READ_INTERVAL_MS 3000
#define INVALID_SENSOR_VALUE INT16_MIN

int16_t temperatureHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t humidityHistoryBuffer[HISTORY_BUFFER_SIZE];
int16_t pressureHistoryBuffer[HISTORY_BUFFER_SIZE];

DigitalButton button(BUTTON_PIN, true);
Adafruit_NeoPixel rgbled(1, RGBLED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
Adafruit_AHTX0 thermometer;
Adafruit_BMP280 barometer;

SensorDataHistory temperatureHistory(temperatureHistoryBuffer, HISTORY_BUFFER_SIZE);
SensorDataHistory humidityHistory(humidityHistoryBuffer, HISTORY_BUFFER_SIZE);
SensorDataHistory pressureHistory(pressureHistoryBuffer, HISTORY_BUFFER_SIZE);
Model model(temperatureHistory, humidityHistory, pressureHistory);
View view(model, display, DISPLAY_WIDTH, DISPLAY_HEIGHT);
SensorManager sensorManager(thermometer, barometer, SENSOR_READ_INTERVAL_MS);

void setup() {
  rgbled.begin();
  rgbled.setBrightness(RGBLED_BRIGHTNESS);
  blink(160, 160, 160, 200);
  blink(160, 160, 160, 200);
  blink(160, 160, 160, 200);

  Serial.begin(SERIAL_SPEED);
  while (!Serial && millis() < 1000);
  Serial.println();
  Serial.println("--");
  Serial.println("Thermohygrometer (built at " __DATE__ " " __TIME__ ")");

  button.begin();
  sensorManager.begin();
  delay(100);

  model.begin();
  view.begin(DISPLAY_I2C_ADDRESS);
}

void loop() {
  static bool needRender = true;

  button.update();
  sensorManager.update();

  if (button.isLongPressed()) {
    view.flip();
    needRender = true;
  }

  if (button.isClicked()) {
    view.switchToNextViewMode();
    needRender = true;
  }

  if (sensorManager.consumeReady()) {
    SensorManager::SensorData data = sensorManager.getSensorData();
    printSensorData(millis(), data);
    model.update(data.temperature, data.humidity, data.pressure);
    needRender = true;
  }

  if (needRender) {
    view.render();
    needRender = false;
  }

  delay(10);
}

void blink(uint8_t r, uint8_t g, uint8_t b, unsigned long durationMs) {
  rgbled.setPixelColor(0, r, g, b);
  rgbled.show();
  delay(durationMs);
  rgbled.setPixelColor(0, 0, 0, 0);
  rgbled.show();
  delay(durationMs);
}

void printSensorData(unsigned long timestamp, const SensorManager::SensorData& data) {
  Serial.print("TS:");
  Serial.print(timestamp);
  Serial.print(" T:");
  printSensorValue(data.temperature);
  Serial.print(" H:");
  printSensorValue(data.humidity);
  Serial.print(" P:");
  printSensorValue(data.pressure);
  Serial.println();
}

void printSensorValue(int16_t value) {
  int16_t intPart = value / 10;
  uint8_t fracPart = abs(value % 10);

  if (value < 0 && intPart == 0) {
    Serial.print("-0.");
  } else {
    Serial.print(intPart);
    Serial.print(".");
  }

  Serial.print(fracPart);
}
