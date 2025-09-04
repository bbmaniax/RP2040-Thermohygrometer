// Thermohygrometer

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_COLOR_WHITE SSD1306_WHITE

#define CHART_OFFSET 16
#define CHART_WIDTH (DISPLAY_WIDTH)
#define CHART_HEIGHT (DISPLAY_HEIGHT - CHART_OFFSET)

Adafruit_AHTX0 thermometer;
Adafruit_BMP280 barometer;
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

int16_t temperatures[DISPLAY_WIDTH] = { 0 };
int16_t humidities[DISPLAY_WIDTH] = { 0 };
int16_t pressures[DISPLAY_WIDTH] = { 0 };

int16_t temperatureMin, temperatureMax;
int16_t humidityMin, humidityMax;
int16_t pressureMin, pressureMax;

int currentPattern = 0;
int patternCounter = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS);
  display.display();
  thermometer.begin();
  barometer.begin();
  delay(1 * 1000);

  measure();
  for (int i = 1; i < DISPLAY_WIDTH; i++) {
    temperatures[i] = temperatures[0];
    humidities[i] = humidities[0];
    pressures[i] = pressures[0];
  }
  temperatureMin = temperatureMax = temperatures[0];
  humidityMin = humidityMax = humidities[0];
  pressureMin = pressureMax = pressures[0];
}

void loop() {
  measure();
  display.clearDisplay();

  switch (currentPattern) {
    case 0:
      displayPattern1();
      break;
    case 1:
      displayPattern2();
      break;
    case 2:
      displayPattern3();
      break;
    case 3:
      displayPattern4();
      break;
  }

  display.display();
  delay(500);

  patternCounter = (patternCounter + 1) % 30;
  if (patternCounter == 0) {
    currentPattern = (currentPattern + 1) % 4;
  }
}

void displayPattern1() {
  display.setTextSize(2);
  display.setTextColor(DISPLAY_COLOR_WHITE);
  printCenter(6, "%.1fC", temperatures[0] / 10.0);
  printCenter(24, "%.1f%%", humidities[0] / 10.0);
  printCenter(42, "%.1fhPa", pressures[0] / 10.0);
}

void displayPattern2() {
  display.setTextSize(2);
  display.setTextColor(DISPLAY_COLOR_WHITE);
  printLeft(0, "%.1fC", temperatures[0] / 10.0);
  drawChart(temperatures, temperatureMin, temperatureMax);
}

void displayPattern3() {
  display.setTextSize(2);
  display.setTextColor(DISPLAY_COLOR_WHITE);
  printLeft(0, "%.1f%%", humidities[0] / 10.0);
  drawChart(humidities, humidityMin, humidityMax);
}

void displayPattern4() {
  display.setTextSize(2);
  display.setTextColor(DISPLAY_COLOR_WHITE);
  printLeft(0, "%.1fhPa", pressures[0] / 10.0);
  drawChart(pressures, pressureMin, pressureMax);
}

void updateMinMax(int16_t* array, int16_t& min_val, int16_t& max_val, int16_t lostValue = -1) {
  if (lostValue != -1 && lostValue != min_val && lostValue != max_val) {
    if (array[0] < min_val) min_val = array[0];
    if (array[0] > max_val) max_val = array[0];
  } else {
    min_val = max_val = array[0];
    for (int i = 1; i < DISPLAY_WIDTH; i++) {
      if (array[i] < min_val) min_val = array[i];
      if (array[i] > max_val) max_val = array[i];
    }
  }
}

void measure() {
  int16_t lostTemp = temperatures[DISPLAY_WIDTH - 1];
  int16_t lostHum = humidities[DISPLAY_WIDTH - 1];
  int16_t lostPres = pressures[DISPLAY_WIDTH - 1];

  memcpy(temperatures + 1, temperatures, sizeof(temperatures) - sizeof(int16_t));
  memcpy(humidities + 1, humidities, sizeof(humidities) - sizeof(int16_t));
  memcpy(pressures + 1, pressures, sizeof(pressures) - sizeof(int16_t));

  sensors_event_t tmp, hum;
  thermometer.getEvent(&hum, &tmp);

  temperatures[0] = tmp.temperature * 10;
  humidities[0] = hum.relative_humidity * 10;
  pressures[0] = barometer.readPressure() / 10.0F;

  updateMinMax(temperatures, temperatureMin, temperatureMax, lostTemp);
  updateMinMax(humidities, humidityMin, humidityMax, lostHum);
  updateMinMax(pressures, pressureMin, pressureMax, lostPres);
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

void printLeft(int y, const char* fmt, float value) {
  char buf[16];
  snprintf(buf, sizeof(buf), fmt, value);

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(DISPLAY_WIDTH - w, y);
  display.print(buf);
}

void drawChart(int16_t* array, int16_t min_val, int16_t max_val) {
  if (max_val - min_val == 0) {
    max_val = min_val + 10;
  }

  for (int x = 0; x < CHART_WIDTH - 1; x++) {
    float v1 = array[CHART_WIDTH - 1 - x] / 10.0;
    float v2 = array[CHART_WIDTH - 1 - (x + 1)] / 10.0;
    float fmin = min_val / 10.0;
    float fmax = max_val / 10.0;
    int y1 = CHART_OFFSET + (int)((fmax - v1) * CHART_HEIGHT / (fmax - fmin));
    int y2 = CHART_OFFSET + (int)((fmax - v2) * CHART_HEIGHT / (fmax - fmin));
    if (y1 < CHART_OFFSET) y1 = CHART_OFFSET;
    if (y1 > CHART_OFFSET + CHART_HEIGHT - 1) y1 = CHART_OFFSET + CHART_HEIGHT - 1;
    if (y2 < CHART_OFFSET) y2 = CHART_OFFSET;
    if (y2 > CHART_OFFSET + CHART_HEIGHT - 1) y2 = CHART_OFFSET + CHART_HEIGHT - 1;
    display.drawLine(x, y1, x + 1, y2, DISPLAY_COLOR_WHITE);
  }
}
