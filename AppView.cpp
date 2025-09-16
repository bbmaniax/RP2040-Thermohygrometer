/*
  AppView Display Library

  A library for rendering sensor data and charts
  on OLED displays.
*/

#include "AppView.h"

#include <Arduino.h>
#include <Adafruit_GFX.h>

#include "DebugSerial.h"
#include "History.h"

AppView::AppView(Adafruit_SSD1306& display, size_t width, size_t height) : display(display), width(width), height(height) {}

void AppView::render(int patternIndex, History& temperatureHistory, History& humidityHistory, History& pressureHistory) {
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

size_t AppView::getWidth() const {
  return width;
}

size_t AppView::getHeight() const {
  return height;
}

void AppView::renderCurrentSensorData(History& temperatureHistory, History& humidityHistory, History& pressureHistory) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 8);
  display.println(String(temperatureHistory.getValue(0)) + "C");
  display.println(String(humidityHistory.getValue(0)) + "%");
  display.println(String(pressureHistory.getValue(0)) + "hPa");
  display.display();
}

void AppView::renderTemperatureChart(History& temperatureHistory) {
  display.clearDisplay();
  drawChart(temperatureHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(String(temperatureHistory.getValue(0)) + "C");
  display.display();
}

void AppView::renderHumidityChart(History& humidityHistory) {
  display.clearDisplay();
  drawChart(humidityHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(String(humidityHistory.getValue(0)) + "%");
  display.display();
}

void AppView::renderPressureChart(History& pressureHistory) {
  display.clearDisplay();
  drawChart(pressureHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(String(pressureHistory.getValue(0)) + "hPa");
  display.display();
}

void AppView::drawChart(History& history) {
  float minValue = history.getMinValue();
  float maxValue = history.getMaxValue();

  float tempRange = maxValue - minValue;
  if (tempRange < 0.1f) { tempRange = 0.1f; }

  for (uint8_t x = 0; x < getWidth() - 1; x++) {
    float currentValue = history.getValue(getWidth() - 1 - x);
    float nextValue = history.getValue(getWidth() - 2 - x);

    uint8_t currentY = 17 + (uint8_t)((maxValue - currentValue) * (getHeight() - 1 - 17) / tempRange);
    uint8_t nextY = 17 + (uint8_t)((maxValue - nextValue) * (getHeight() - 1 - 17) / tempRange);

    if (currentY >= getHeight()) currentY = getHeight() - 1;
    if (nextY >= getHeight()) nextY = getHeight() - 1;

    display.drawLine(x, currentY, x + 1, nextY, SSD1306_WHITE);
  }
}
