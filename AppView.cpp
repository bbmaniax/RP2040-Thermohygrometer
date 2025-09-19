/*
  AppView Display Library

  A library for rendering sensor data and charts
  on OLED displays.
*/

#include "AppView.h"

#include <Arduino.h>
#include <Adafruit_GFX.h>

#include "History.h"

AppView::AppView(Adafruit_SSD1306& display, size_t width, size_t height, uint8_t plotHorizontalSpacing)
  : display(display), width(width), height(height), plotHorizontalStep(plotHorizontalSpacing + 1) {}

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

void AppView::renderCurrentSensorData(History& temperatureHistory, History& humidityHistory, History& pressureHistory) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 8);
  display.printf("%7s", (String(temperatureHistory.getValue(0) / 10.0f, 1) + "C").c_str());
  display.setCursor(10, 24);
  display.printf("%7s", (String(humidityHistory.getValue(0) / 10.0f, 1) + "%").c_str());
  display.setCursor(10, 40);
  display.printf("%9s", (String(pressureHistory.getValue(0) / 10.0f, 1) + "hPa").c_str());
  display.display();
}

void AppView::renderTemperatureChart(History& temperatureHistory) {
  display.clearDisplay();
  drawChart(temperatureHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(String(temperatureHistory.getValue(0) / 10.0f, 1) + "C");
  display.display();
}

void AppView::renderHumidityChart(History& humidityHistory) {
  display.clearDisplay();
  drawChart(humidityHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(String(humidityHistory.getValue(0) / 10.0f, 1) + "%");
  display.display();
}

void AppView::renderPressureChart(History& pressureHistory) {
  display.clearDisplay();
  drawChart(pressureHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(String(pressureHistory.getValue(0) / 10.0f, 1) + "hPa");
  display.display();
}

void AppView::drawChart(History& history) {
  int16_t minValue = history.getMinValue();
  int16_t maxValue = history.getMaxValue();

  int16_t tempRange = maxValue - minValue;
  if (tempRange < 1) { tempRange = 1; }

  uint8_t step = plotHorizontalStep;
  for (uint8_t i = 0; i < width - 1; i++) {
    int16_t currentValue = history.getValue(i);
    int16_t nextValue = history.getValue(i + 1);

    uint8_t currentY = 17 + (uint8_t)((maxValue - currentValue) * (height - 1 - 17) / tempRange);
    uint8_t nextY = 17 + (uint8_t)((maxValue - nextValue) * (height - 1 - 17) / tempRange);

    if (currentY >= height) currentY = height - 1;
    if (nextY >= height) nextY = height - 1;

    display.drawLine(width - (i * step) - 1, currentY, width - ((i + 1) * step) - 1, nextY, SSD1306_WHITE);
  }
}
