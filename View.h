/*
  View Display Library

  A library for rendering sensor data and charts
  on OLED displays.
*/

#ifndef VIEW_H
#define VIEW_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class History;

class View {
public:
  View(Adafruit_SSD1306& display);

public:
  void render(int patternIndex, History& temperatureHistory, History& humidityHistory, History& pressureHistory);

private:
  void renderCurrentSensorData(History& temperatureHistory, History& humidityHistory, History& pressureHistory);
  void renderTemperatureChart(History& temperatureHistory);
  void renderHumidityChart(History& humidityHistory);
  void renderPressureChart(History& pressureHistory);

  void drawChart(History& history);
  void drawSensorValue(float value, const char* unit);

private:
  Adafruit_SSD1306& display;
};

#endif
