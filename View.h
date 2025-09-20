// View - Manages the display of sensor data and charts.

#ifndef __APP_VIEW_H__
#define __APP_VIEW_H__

#include <Arduino.h>

class Adafruit_SSD1306;
class History;
class ViewState;

class View {
public:
  View(ViewState& viewState, Adafruit_SSD1306& display, size_t width, size_t height, uint8_t plotHorizontalSpacing);

  void flipDisplay();
  void render(History& temperatureHistory, History& humidityHistory, History& pressureHistory);

private:
  ViewState& viewState;
  Adafruit_SSD1306& display;
  size_t width;
  size_t height;
  uint8_t plotHorizontalStep;

  void renderCurrentSensorData(History& temperatureHistory, History& humidityHistory, History& pressureHistory);
  void renderTemperatureChart(History& temperatureHistory);
  void renderHumidityChart(History& humidityHistory);
  void renderPressureChart(History& pressureHistory);
  void drawSensorValue(float value, const char* unit);
  void drawChart(History& history);
};

#endif  // __APP_VIEW_H__
