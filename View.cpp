/*
  View Display Library

  A library for rendering sensor data and charts
  on OLED displays.
*/

#include "View.h"

#include <Arduino.h>
#include <Adafruit_GFX.h>

#include "DebugSerial.h"
#include "History.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_COLOR_WHITE SSD1306_WHITE

View::View(Adafruit_SSD1306& display) : display(display) {}

void View::render(int patternIndex, History& temperatureHistory, History& humidityHistory, History& pressureHistory) {
  switch (patternIndex) {
    case 0:
      renderCurrentSensorData(temperatureHistory, humidityHistory, pressureHistory);
      break;
    case 1:
      renderTemperatureChart(temperatureHistory);
      break;
    case 2:
      renderHumidityChart(humidityHistory);
      break;
    case 3:
      renderPressureChart(pressureHistory);
      break;
    default:
      renderCurrentSensorData(temperatureHistory, humidityHistory, pressureHistory);
      break;
  }
}

void View::renderCurrentSensorData(History& temperatureHistory, History& humidityHistory, History& pressureHistory) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(DISPLAY_COLOR_WHITE);
  display.setCursor(0, 0);
  display.print(temperatureHistory.getValue(0));
  display.println("C");
  display.print(humidityHistory.getValue(0));
  display.println("%");
  display.print(pressureHistory.getValue(0));
  display.println("hPa");
  display.display();
}

void View::renderTemperatureChart(History& temperatureHistory) {
  display.clearDisplay();
  drawChart(temperatureHistory);
  drawSensorValue(temperatureHistory.getValue(0), "C");
  display.display();
}

void View::renderHumidityChart(History& humidityHistory) {
  display.clearDisplay();
  drawChart(humidityHistory);
  drawSensorValue(humidityHistory.getValue(0), "%");
  display.display();
}

void View::renderPressureChart(History& pressureHistory) {
  display.clearDisplay();
  drawChart(pressureHistory);
  drawSensorValue(pressureHistory.getValue(0), "hPa");
  display.display();
}

void View::drawChart(History& history) {
  float minValue = history.getMinValue();
  float maxValue = history.getMaxValue();

  float tempRange = maxValue - minValue;
  if (tempRange < 0.1f) { tempRange = 0.1f; }

  for (uint8_t x = 0; x < DISPLAY_WIDTH - 1; x++) {
    float currentValue = history.getValue(DISPLAY_WIDTH - 1 - x);
    float nextValue = history.getValue(DISPLAY_WIDTH - 2 - x);

    uint8_t currentY = 17 + (uint8_t)((maxValue - currentValue) * (DISPLAY_HEIGHT - 1 - 17) / tempRange);
    uint8_t nextY = 17 + (uint8_t)((maxValue - nextValue) * (DISPLAY_HEIGHT - 1 - 17) / tempRange);

    if (currentY >= DISPLAY_HEIGHT) currentY = DISPLAY_HEIGHT - 1;
    if (nextY >= DISPLAY_HEIGHT) nextY = DISPLAY_HEIGHT - 1;

    display.drawLine(x, currentY, x + 1, nextY, DISPLAY_COLOR_WHITE);
  }
}

void View::drawSensorValue(float value, const char* unit) {
  String text = String(value) + unit;
  display.setTextColor(DISPLAY_COLOR_WHITE);
  display.setCursor(0, 0);
  display.print(value);
  display.print(unit);
}
