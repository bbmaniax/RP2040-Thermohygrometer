// View

#ifndef __APP_VIEW_H__
#define __APP_VIEW_H__

#include <Arduino.h>

class Adafruit_SSD1306;
class History;
class Model;

class View {
public:
  View(Model& model, Adafruit_SSD1306& display, size_t width, size_t height, uint8_t horizontalSpacing = 1, bool flipped = false);

  void begin(uint8_t i2cAddress, bool displayOn = true);
  void switchToNextViewMode();
  void flip();
  void render();

private:
  typedef enum {
    VIEW_MODE_ALL_CHARTS = 0,
    VIEW_MODE_TEMPERATURE_CHART,
    VIEW_MODE_HUMIDITY_CHART,
    VIEW_MODE_PRESSURE_CHART,
    VIEW_MODE_ALL_TEXT,
    VIEW_MODE_COUNT
  } ViewMode;

  void renderAllCharts(Adafruit_SSD1306& display);
  void renderTemperatureChart(Adafruit_SSD1306& display);
  void renderHumidityChart(Adafruit_SSD1306& display);
  void renderPressureChart(Adafruit_SSD1306& display);
  void renderAllText(Adafruit_SSD1306& display);

  void drawChart(Adafruit_SSD1306& display, int16_t x, int16_t y, int16_t w, int16_t h, History& history);

  Model& model;
  ViewMode viewMode;
  Adafruit_SSD1306& display;
  size_t width;
  size_t height;
  uint8_t plotHorizontalStep;
  bool flipped;
  bool initialFlipped;
};

#endif  // __APP_VIEW_H__
