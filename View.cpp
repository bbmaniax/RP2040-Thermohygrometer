// View - Manages the display of sensor data and charts.

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "History.h"
#include "View.h"
#include "ViewState.h"

View::View(ViewState& viewState, Adafruit_SSD1306& display, size_t width, size_t height, uint8_t plotHorizontalSpacing)
    : viewState(viewState), display(display), width(width), height(height), plotHorizontalStep(plotHorizontalSpacing + 1) {}

bool View::begin(bool displayOn) {
  viewState.begin();
  bool ok = display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (ok) {
    display.display();
  }
  return ok;
}

void View::render(History& temperatureHistory, History& humidityHistory, History& pressureHistory) {
  switch (viewState.getViewMode()) {
    case 0: renderCurrentSensorData(temperatureHistory, humidityHistory, pressureHistory); break;
    case 1: renderTemperatureChart(temperatureHistory); break;
    case 2: renderHumidityChart(humidityHistory); break;
    case 3: renderPressureChart(pressureHistory); break;
    default: renderCurrentSensorData(temperatureHistory, humidityHistory, pressureHistory); break;
  }
}

void View::renderCurrentSensorData(History& temperatureHistory, History& humidityHistory, History& pressureHistory) {
  display.clearDisplay();
  display.setRotation(viewState.isDisplayFlipped() ? 2 : 0);
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

void View::renderTemperatureChart(History& temperatureHistory) {
  display.clearDisplay();
  display.setRotation(viewState.isDisplayFlipped() ? 2 : 0);
  drawChart(temperatureHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(String(temperatureHistory.getValue(0) / 10.0f, 1) + "C");
  display.display();
}

void View::renderHumidityChart(History& humidityHistory) {
  display.clearDisplay();
  display.setRotation(viewState.isDisplayFlipped() ? 2 : 0);
  drawChart(humidityHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(String(humidityHistory.getValue(0) / 10.0f, 1) + "%");
  display.display();
}

void View::renderPressureChart(History& pressureHistory) {
  display.clearDisplay();
  display.setRotation(viewState.isDisplayFlipped() ? 2 : 0);
  drawChart(pressureHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(String(pressureHistory.getValue(0) / 10.0f, 1) + "hPa");
  display.display();
}

void View::drawChart(History& history) {
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
