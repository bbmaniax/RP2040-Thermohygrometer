// View.cpp - View for Thermohygrometer

#include "View.h"

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "Model.h"
#include "SensorDataHistory.h"

View::View(Model& model, Adafruit_SSD1306& display, size_t width, size_t height, uint8_t horizontalSpacing)
    : model(model), viewMode(VIEW_MODE_ALL_TEXT), display(display), width(width), height(height), plotHorizontalStep(horizontalSpacing + 1), flipped(false) {
}

void View::begin(uint8_t displayI2CAddress, bool displayOn) {
  viewMode = VIEW_MODE_ALL_CHARTS;
  if (!display.begin(SSD1306_SWITCHCAPVCC, displayI2CAddress)) {
    Serial.println("Failed to initialize display");
  }
  if (displayOn) {
    display.display();
  }
}

void View::switchToNextViewMode() {
  viewMode = static_cast<ViewMode>((static_cast<int>(viewMode) + 1) % VIEW_MODE_COUNT);
}

void View::flip() {
  flipped = !flipped;
}

void View::render() {
  switch (viewMode) {
    case VIEW_MODE_ALL_CHARTS:
      renderAllCharts(display);
      break;
    case VIEW_MODE_TEMPERATURE_CHART:
      renderTemperatureChart(display);
      break;
    case VIEW_MODE_HUMIDITY_CHART:
      renderHumidityChart(display);
      break;
    case VIEW_MODE_PRESSURE_CHART:
      renderPressureChart(display);
      break;
    case VIEW_MODE_ALL_TEXT:
      renderAllText(display);
      break;
    default:
      renderAllText(display);
      break;
  }
}

void View::renderAllCharts(Adafruit_SSD1306& display) {
  const uint8_t chartHeight = 20;
  const uint8_t textHeight = 8;
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawSensorData(model.getTemperature(), "C", {0, 0, (int16_t)width, chartHeight}, TEXT_SIZE_SMALL, HALIGN_LEFT, VALIGN_TOP, true);
  drawSensorData(model.getHumidity(), "%", {0, 21, (int16_t)width, chartHeight}, TEXT_SIZE_SMALL, HALIGN_LEFT, VALIGN_TOP, true);
  drawSensorData(model.getPressure(), "hPa", {0, 42, (int16_t)width, chartHeight}, TEXT_SIZE_SMALL, HALIGN_LEFT, VALIGN_TOP, true);
  drawSensorDataHistory(model.getTemperatureHistory(), {0, (int16_t)textHeight, (int16_t)width, (int16_t)(chartHeight - textHeight)});
  drawSensorDataHistory(model.getHumidityHistory(), {0, (int16_t)(21 + textHeight), (int16_t)width, (int16_t)(chartHeight - textHeight)});
  drawSensorDataHistory(model.getPressureHistory(), {0, (int16_t)(42 + textHeight), (int16_t)width, (int16_t)(chartHeight - textHeight)});
  display.display();
}

void View::renderTemperatureChart(Adafruit_SSD1306& display) {
  const uint8_t textHeight = 16;
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawSensorDataHistory(model.getTemperatureHistory(), {0, (int16_t)textHeight, (int16_t)width, (int16_t)(height - textHeight)});
  drawSensorData(model.getTemperature(), "C", {0, 0, (int16_t)width, (int16_t)textHeight}, TEXT_SIZE_MEDIUM, HALIGN_LEFT, VALIGN_TOP, true);
  display.display();
}

void View::renderHumidityChart(Adafruit_SSD1306& display) {
  const uint8_t textHeight = 16;
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawSensorDataHistory(model.getHumidityHistory(), {0, (int16_t)textHeight, (int16_t)width, (int16_t)(height - textHeight)});
  drawSensorData(model.getHumidity(), "%", {0, 0, (int16_t)width, (int16_t)textHeight}, TEXT_SIZE_MEDIUM, HALIGN_LEFT, VALIGN_TOP, true);
  display.display();
}

void View::renderPressureChart(Adafruit_SSD1306& display) {
  const uint8_t textHeight = 16;
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);
  drawSensorDataHistory(model.getPressureHistory(), {0, (int16_t)textHeight, (int16_t)width, (int16_t)(height - textHeight)});
  drawSensorData(model.getPressure(), "hPa", {0, 0, (int16_t)width, (int16_t)textHeight}, TEXT_SIZE_MEDIUM, HALIGN_LEFT, VALIGN_TOP, true);
  display.display();
}

void View::renderAllText(Adafruit_SSD1306& display) {
  display.clearDisplay();
  display.setRotation(flipped ? 2 : 0);

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  int16_t temperature = model.getTemperature();
  int16_t humidity = model.getHumidity();
  int16_t pressure = model.getPressure();

  static char textBuffer[16];
  int16_t x1, y1;
  uint16_t w, h;
  const int16_t dotX = width / 2;

  if (IS_VALID_SENSOR_VALUE(temperature)) {
    int16_t intPart = temperature / 10;
    uint8_t fracPart = abs(temperature % 10);
    if (temperature < 0 && intPart == 0) {
      snprintf(textBuffer, sizeof(textBuffer), "  -0");
    } else {
      snprintf(textBuffer, sizeof(textBuffer), "%4d", intPart);
    }
    display.getTextBounds(textBuffer, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(dotX - w, 8);
    display.print(textBuffer);
    snprintf(textBuffer, sizeof(textBuffer), ".%d", fracPart);
    display.print(textBuffer);
    display.getTextBounds(textBuffer, 0, 0, &x1, &y1, &w, &h);
    int16_t unitX = dotX + w;
    display.setTextSize(1);
    display.drawCircle(unitX + 2, 9, 1, SSD1306_WHITE);
    display.setCursor(unitX + 5, 8);
    display.print("C");
  } else {
    display.setCursor(dotX - 24, 8);
    display.print("--.-");
  }

  display.setTextSize(2);
  if (IS_VALID_SENSOR_VALUE(humidity)) {
    int16_t intPart = humidity / 10;
    uint8_t fracPart = abs(humidity % 10);
    snprintf(textBuffer, sizeof(textBuffer), "%4d", intPart);
    display.getTextBounds(textBuffer, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(dotX - w, 24);
    display.print(textBuffer);
    snprintf(textBuffer, sizeof(textBuffer), ".%d", fracPart);
    display.print(textBuffer);
    display.getTextBounds(textBuffer, 0, 0, &x1, &y1, &w, &h);
    int16_t unitX = dotX + w;
    display.setTextSize(1);
    display.setCursor(unitX, 24);
    display.print("%");
  } else {
    display.setCursor(dotX - 24, 24);
    display.print("--.-");
  }

  display.setTextSize(2);
  if (IS_VALID_SENSOR_VALUE(pressure)) {
    int16_t intPart = pressure / 10;
    uint8_t fracPart = abs(pressure % 10);
    snprintf(textBuffer, sizeof(textBuffer), "%4d", intPart);
    display.getTextBounds(textBuffer, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(dotX - w, 40);
    display.print(textBuffer);
    snprintf(textBuffer, sizeof(textBuffer), ".%d", fracPart);
    display.print(textBuffer);
    display.getTextBounds(textBuffer, 0, 0, &x1, &y1, &w, &h);
    int16_t unitX = dotX + w;
    display.setTextSize(1);
    display.setCursor(unitX, 40);
    display.print("hPa");
  } else {
    display.setCursor(dotX - 24, 40);
    display.print("--.-");
  }

  display.display();
}

void View::drawSensorData(int16_t value, const char* unit, const Rect& rect, TextSize textSize, HorizontalAlign hAlign, VerticalAlign vAlign, bool withBackground) {
  static char valueText[8];
  strcpy(valueText, "--.-");

  if (IS_VALID_SENSOR_VALUE(value)) {
    int16_t intPart = value / 10;
    uint8_t fracPart = abs(value % 10);
    if (value < 0 && intPart == 0) {
      snprintf(valueText, sizeof(valueText), "-0.%u", fracPart);
    } else {
      snprintf(valueText, sizeof(valueText), "%d.%u", intPart, fracPart);
    }
  }

  TextSize valueSize = textSize;
  TextSize unitSize = (textSize >= TEXT_SIZE_MEDIUM) ? static_cast<TextSize>(textSize - 1) : textSize;

  int16_t x1, y1;
  uint16_t valueW, valueH, unitW, unitH;

  display.setTextSize(valueSize);
  display.getTextBounds(valueText, 0, 0, &x1, &y1, &valueW, &valueH);

  display.setTextSize(unitSize);
  display.getTextBounds(unit, 0, 0, &x1, &y1, &unitW, &unitH);

  uint16_t totalW = valueW + unitW + 3;
  uint16_t totalH = valueH;

  int16_t cursorX = rect.x;
  int16_t cursorY = rect.y;

  switch (hAlign) {
    case HALIGN_LEFT:
      break;
    case HALIGN_CENTER:
      cursorX += (rect.w - totalW) / 2;
      break;
    case HALIGN_RIGHT:
      cursorX += rect.w - totalW;
      break;
  }

  switch (vAlign) {
    case VALIGN_TOP:
      break;
    case VALIGN_CENTER:
      cursorY += (rect.h - totalH) / 2;
      break;
    case VALIGN_BOTTOM:
      cursorY += rect.h - totalH;
      break;
  }

  if (withBackground) {
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  } else {
    display.setTextColor(SSD1306_WHITE);
  }

  display.setTextSize(valueSize);
  display.setCursor(cursorX, cursorY);
  display.print(valueText);

  if (strcmp(unit, "C") == 0) {
    int16_t degreeY = cursorY + (valueSize == 1 ? 1 : (valueSize == 2 ? 2 : 3));
    display.drawCircle(cursorX + valueW + 2, degreeY, 1, SSD1306_WHITE);
  }

  display.setTextSize(unitSize);
  display.setCursor(cursorX + valueW + (strcmp(unit, "C") == 0 ? 5 : 0), cursorY);
  display.print(unit);
}

void View::drawSensorDataHistory(SensorDataHistory& history, const Rect& rect) {
  if (rect.w <= 0 || rect.h <= 0 || plotHorizontalStep == 0) {
    return;
  }

  const int16_t chartX = rect.x;
  const int16_t chartY = rect.y;
  const int16_t chartW = rect.w;
  const int16_t chartH = rect.h;

  size_t count = history.getCount();

  if (count >= 2) {
    size_t maxDataPoints = (chartW + plotHorizontalStep - 1) / plotHorizontalStep + 1;
    size_t drawCount = count < maxDataPoints ? count : maxDataPoints;

    int16_t minValue, maxValue;
    history.getMinMaxValue(drawCount, minValue, maxValue);

    if (!IS_VALID_SENSOR_VALUE(minValue) || !IS_VALID_SENSOR_VALUE(maxValue)) {
      return;
    }

    int16_t range = maxValue - minValue;
    if (range < 1) {
      range = 1;
    }

    for (size_t i = 0; i < drawCount - 1; i++) {
      int16_t currentValue = history.getValue(i);
      int16_t nextValue = history.getValue(i + 1);

      if (IS_VALID_SENSOR_VALUE(currentValue) && IS_VALID_SENSOR_VALUE(nextValue)) {
        int16_t currentY = chartY + (int16_t)(((int32_t)(maxValue - currentValue) * (chartH - 1)) / range);
        int16_t nextY = chartY + (int16_t)(((int32_t)(maxValue - nextValue) * (chartH - 1)) / range);

        int16_t currentX = chartX + chartW - 1 - (i * plotHorizontalStep);
        int16_t nextX = chartX + chartW - 1 - ((i + 1) * plotHorizontalStep);
        display.drawLine(currentX, currentY, nextX, nextY, SSD1306_WHITE);
      }
    }
  }
}
