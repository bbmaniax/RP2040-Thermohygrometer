// View - Manages the display of sensor data and charts.

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "DebugSerial.h"
#include "History.h"
#include "View.h"

View::View(Adafruit_SSD1306& display, size_t width, size_t height, uint8_t horizontalSpacing, bool flipped)
    : viewMode(VIEW_MODE_ALL), display(display), width(width), height(height), plotHorizontalStep(horizontalSpacing + 1), flipped(flipped), initialFlipped(flipped) {}

void View::begin(uint8_t i2cAddress, bool displayOn) {
  DEBUG_SERIAL_PRINTLN("Initializing View: displayOn=" + String(displayOn));
  viewMode = VIEW_MODE_ALL;
  flipped = initialFlipped;
  if (!display.begin(SSD1306_SWITCHCAPVCC, i2cAddress)) { DEBUG_SERIAL_PRINTLN("Failed to initialize display"); }
  if (displayOn) { display.display(); }
}

void View::switchToNextViewMode() {
  DEBUG_SERIAL_PRINTLN("Switching to next view mode: current mode=" + String(static_cast<int>(viewMode)));
  viewMode = static_cast<ViewMode>((static_cast<int>(viewMode) + 1) % VIEW_MODE_COUNT);
  DEBUG_SERIAL_PRINTLN("New view mode=" + String(static_cast<int>(viewMode)));
}

void View::flip() {
  DEBUG_SERIAL_PRINTLN("Flipping display orientation: current flipped=" + String(flipped));
  flipped = !flipped;
  DEBUG_SERIAL_PRINTLN("New flipped=" + String(flipped));
}

void View::render(History& temperatureHistory, History& humidityHistory, History& pressureHistory) {
  // DEBUG_SERIAL_PRINTLN("Rendering view: mode=" + String(static_cast<int>(viewMode)) + ", flipped=" + String(flipped));
  switch (viewMode) {
    case VIEW_MODE_ALL: renderCurrentSensorData(temperatureHistory, humidityHistory, pressureHistory); break;
    case VIEW_MODE_TEMPERATURE: renderTemperatureChart(temperatureHistory); break;
    case VIEW_MODE_HUMIDITY: renderHumidityChart(humidityHistory); break;
    case VIEW_MODE_PRESSURE: renderPressureChart(pressureHistory); break;
    default: renderCurrentSensorData(temperatureHistory, humidityHistory, pressureHistory); break;
  }
}

void View::renderCurrentSensorData(History& temperatureHistory, History& humidityHistory, History& pressureHistory) {
  // DEBUG_SERIAL_PRINTLN("Rendering current sensor data");
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
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
  // DEBUG_SERIAL_PRINTLN("Rendering temperature chart");
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawChart(temperatureHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(String(temperatureHistory.getValue(0) / 10.0f, 1) + "C");
  display.display();
}

void View::renderHumidityChart(History& humidityHistory) {
  // DEBUG_SERIAL_PRINTLN("Rendering humidity chart");
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawChart(humidityHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(String(humidityHistory.getValue(0) / 10.0f, 1) + "%");
  display.display();
}

void View::renderPressureChart(History& pressureHistory) {
  // DEBUG_SERIAL_PRINTLN("Rendering pressure chart");
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawChart(pressureHistory);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(String(pressureHistory.getValue(0) / 10.0f, 1) + "hPa");
  display.display();
}

void View::drawChart(History& history) {
  DEBUG_SERIAL_PRINTLN("Drawing chart");
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
