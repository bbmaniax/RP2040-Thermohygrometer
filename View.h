// View.h - View for Thermohygrometer

#pragma once

#ifndef VIEW_H
#  define VIEW_H

#  include <Arduino.h>

class Adafruit_SSD1306;
class SensorDataHistory;
class Model;

class View {
 public:
  struct Rect {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
  };

  enum ViewMode {
    VIEW_MODE_ALL_CHARTS = 0,
    VIEW_MODE_TEMPERATURE_CHART,
    VIEW_MODE_HUMIDITY_CHART,
    VIEW_MODE_PRESSURE_CHART,
    VIEW_MODE_ALL_TEXT,
    VIEW_MODE_COUNT
  };

  enum HorizontalAlign {
    HALIGN_LEFT,
    HALIGN_CENTER,
    HALIGN_RIGHT,
  };

  enum VerticalAlign {
    VALIGN_TOP,
    VALIGN_CENTER,
    VALIGN_BOTTOM,
  };

  enum TextSize {
    TEXT_SIZE_SMALL = 1,
    TEXT_SIZE_MEDIUM = 2,
    TEXT_SIZE_LARGE = 3,
  };

  View(Model& model, Adafruit_SSD1306& display, size_t width, size_t height, uint8_t horizontalSpacing = 1);

  void begin(uint8_t i2cAddress, bool displayOn = true);
  void switchToNextViewMode();
  void flip();
  void render();

 private:
  void renderAllCharts(Adafruit_SSD1306& display);
  void renderTemperatureChart(Adafruit_SSD1306& display);
  void renderHumidityChart(Adafruit_SSD1306& display);
  void renderPressureChart(Adafruit_SSD1306& display);
  void renderAllText(Adafruit_SSD1306& display);
  void drawSensorData(int16_t value, const char* unit, const Rect& rect, TextSize textSize, HorizontalAlign hAlign, VerticalAlign vAlign, bool withBackground);
  void drawSensorDataHistory(SensorDataHistory& history, const Rect& rect);

  Model& model;
  ViewMode viewMode;
  Adafruit_SSD1306& display;
  size_t width;
  size_t height;
  uint8_t plotHorizontalStep;
  bool flipped;
};

#endif  // VIEW_H
