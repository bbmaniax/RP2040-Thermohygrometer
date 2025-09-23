// View - Manages the display of sensor data and charts.

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "DebugSerial.h"
#include "History.h"
#include "Model.h"
#include "View.h"

View::View(Model& model, Adafruit_SSD1306& display, size_t width, size_t height, uint8_t horizontalSpacing, bool flipped)
    : model(model), viewMode(VIEW_MODE_ALL_TEXT), display(display), width(width), height(height), plotHorizontalStep(horizontalSpacing + 1), flipped(flipped), initialFlipped(flipped) {}

void View::begin(uint8_t i2cAddress, bool displayOn) {
  DEBUG_SERIAL_PRINTLN("Initializing View: displayOn=" + String(displayOn));
  viewMode = VIEW_MODE_ALL_TEXT;
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

void View::render() {
  // DEBUG_SERIAL_PRINTLN("Rendering view: mode=" + String(static_cast<int>(viewMode)) + ", flipped=" + String(flipped));
  switch (viewMode) {
    case VIEW_MODE_ALL_TEXT: renderAllText(display); break;
    case VIEW_MODE_TEMPERATURE_CHART: renderTemperatureChart(display); break;
    case VIEW_MODE_HUMIDITY_CHART: renderHumidityChart(display); break;
    case VIEW_MODE_PRESSURE_CHART: renderPressureChart(display); break;
    default: renderAllText(display); break;
  }
}

void View::renderAllText(Adafruit_SSD1306& display) {
  // DEBUG_SERIAL_PRINTLN("Rendering all text");
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 8);
  display.printf("%7s", (String(model.getTemperature() / 10.0f, 1) + "C").c_str());
  display.setCursor(10, 24);
  display.printf("%7s", (String(model.getHumidity() / 10.0f, 1) + "%").c_str());
  display.setCursor(10, 40);
  display.printf("%9s", (String(model.getPressure() / 10.0f, 1) + "hPa").c_str());
  display.display();
}

void View::renderTemperatureChart(Adafruit_SSD1306& display) {
  // DEBUG_SERIAL_PRINTLN("Rendering temperature chart");
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawChart(display, model.getTemperatureHistory());
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(String(model.getTemperature() / 10.0f, 1) + "C");
  display.display();
}

void View::renderHumidityChart(Adafruit_SSD1306& display) {
  // DEBUG_SERIAL_PRINTLN("Rendering humidity chart");
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawChart(display, model.getHumidityHistory());
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(String(model.getHumidity() / 10.0f, 1) + "%");
  display.display();
}

void View::renderPressureChart(Adafruit_SSD1306& display) {
  // DEBUG_SERIAL_PRINTLN("Rendering pressure chart");
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawChart(display, model.getPressureHistory());
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(String(model.getPressure() / 10.0f, 1) + "hPa");
  display.display();
}

void View::drawChart(Adafruit_SSD1306& display, History& history) {
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
