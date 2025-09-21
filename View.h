// View - Manages the display of sensor data and charts.

#ifndef __APP_VIEW_H__
#define __APP_VIEW_H__

#include <Arduino.h>

class Adafruit_SSD1306;
class History;

class View {
public:
  View(Adafruit_SSD1306& display, size_t width, size_t height, uint8_t plotHorizontalSpacing);

  bool begin(bool displayOn = true);
  void switchToNextViewMode();
  void flipDisplay();
  void render(History& temperatureHistory, History& humidityHistory, History& pressureHistory);

private:
  typedef enum {
    VIEW_MODE_ALL = 0,
    VIEW_MODE_TEMPERATURE,
    VIEW_MODE_HUMIDITY,
    VIEW_MODE_PRESSURE,
    VIEW_MODE_COUNT
  } ViewMode;

  void renderCurrentSensorData(History& temperatureHistory, History& humidityHistory, History& pressureHistory);
  void renderTemperatureChart(History& temperatureHistory);
  void renderHumidityChart(History& humidityHistory);
  void renderPressureChart(History& pressureHistory);
  void drawSensorValue(float value, const char* unit);
  void drawChart(History& history);

  Adafruit_SSD1306& display;
  size_t width;
  size_t height;
  uint8_t plotHorizontalStep;

  ViewMode viewMode;
  bool displayFlipped;
};

#endif  // __APP_VIEW_H__
