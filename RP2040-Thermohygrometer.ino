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

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_COLOR_WHITE SSD1306_WHITE

Adafruit_AHTX0 thermometer;
Adafruit_BMP280 barometer;
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS);
  display.display();

  thermometer.begin();
  barometer.begin();

  delay(3 * 1000);
}

void loop() {
  sensors_event_t humidity, temperature;
  thermometer.getEvent(&humidity, &temperature);

  float pressure = barometer.readPressure() / 100.0F;

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(DISPLAY_COLOR_WHITE);
  printCenter(8, "%.1fC", temperature.temperature);
  printCenter(24, "%.1f%%", humidity.relative_humidity);
  printCenter(40, "%.1fhPa", pressure);
  display.display();

  delay(5 * 1000);
}

void printCenter(int y, const char* fmt, float value) {
  char buf[16];
  snprintf(buf, sizeof(buf), fmt, value);

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((DISPLAY_WIDTH - w) / 2, y);
  display.print(buf);
}
