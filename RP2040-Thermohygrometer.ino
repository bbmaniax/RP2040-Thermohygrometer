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
  Serial.println("Thermohygrometer (build " + timestamp() + ")");

  button.begin();
  sensorManager.begin();
  delay(100);

  model.begin();
  view.begin(DISPLAY_I2C_ADDRESS);

  if (digitalRead(BUTTON_PIN) == LOW) {
    scan(Wire, display);
    while (digitalRead(BUTTON_PIN) == LOW);
  }
}

void loop() {
  static bool needRender = true;

  button.update();
  sensorManager.update();

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

  if (sensorManager.isReady()) {
    // DEBUG_SERIAL_PRINTLN("Time to read sensors");
    SensorManager::SensorData data = sensorManager.getSensorData();
    model.update(data.temperature, data.humidity, data.pressure);
    needRender = true;
  }

  if (needRender) {
    String line =                                         //
      "TS:" + String(millis())                            //
      + " T:" + String(model.getTemperature() / 10.0f, 1)  //
      + " H:" + String(model.getHumidity() / 10.0f, 1)     //
      + " P:" + String(model.getPressure() / 10.0f, 1);    //
    Serial.println(line);
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

String timestamp() {
  const char* m = "JanFebMarAprMayJunJulAugSepOctNovDec";
  char mon[4];
  int d, y, hh, mm, ss;
  sscanf(__DATE__, "%3s %d %d", mon, &d, &y);
  sscanf(__TIME__, "%d:%d:%d", &hh, &mm, &ss);
  int mo = (strstr(m, mon) - m) / 3 + 1;

  char buf[16];
  sprintf(buf, "%04d%02d%02d.%02d%02d%02d", y, mo, d, hh, mm, ss);
  return String(buf);
}

void scan(TwoWire& wire, Adafruit_SSD1306& display) {
  Serial.println("SCANNING");
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
      Serial.println(address, HEX);
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
    Serial.println("NO DEVICES");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("NO DEVICES");
    display.display();
  }
}
