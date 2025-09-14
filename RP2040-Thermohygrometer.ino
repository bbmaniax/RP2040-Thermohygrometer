/*
  Thermohygrometer

  A thermohygrometer with OLED display
  for temperature, humidity, and pressure monitoring.
*/

#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "button.h"
#include "gnd.h"
#include "history.h"
#include "serial.h"

#define BUTTON1_GND_PIN 28

#define BUTTON1_INPUT_PIN 26
#define BUTTON2_INPUT_PIN 29

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_COLOR_WHITE SSD1306_WHITE

GND gnd1(BUTTON1_GND_PIN);

Button button1(BUTTON1_INPUT_PIN);
Button button2(BUTTON2_INPUT_PIN);

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

Adafruit_AHTX0 thermometer;
Adafruit_BMP280 barometer;

History temperatureHistory(DISPLAY_WIDTH + 1);
History humidityHistory(DISPLAY_WIDTH + 1);
History pressureHistory(DISPLAY_WIDTH + 1);

typedef void (*showPatternFunc_t)();

void showPattern1();
void showPattern2();
void showPattern3();
void showPattern4();

showPatternFunc_t showPatternFuncs[] = {
  showPattern1,
  showPattern2,
  showPattern3,
  showPattern4,
};

#define DISPLAY_UPDATE_INTERVAL_MS 3000

unsigned long lastMillis = 0;
uint8_t patternIndex = 0;
bool displayFlipped = false;

void setup() {
  SERIAL_BEGIN(115200);
  WAIT_FOR_SERIAL();
  SERIAL_PRINTLN();
  SERIAL_PRINTLN("--");

  gnd1.begin();
  button1.begin();
  button2.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS)) {
    SERIAL_PRINTLN("Failed to initialize display!");
    abend();
  }
  display.display();

  if (!thermometer.begin()) {
    SERIAL_PRINTLN("Failed to initialize AHTX0!");
    abend();
  }
  if (!barometer.begin()) {
    SERIAL_PRINTLN("Failed to initialize BMP280!");
    abend();
  }

  float temperature, humidity, pressure;
  if (readSensors(&temperature, &humidity, &pressure)) {
    temperatureHistory.fill((int16_t)temperature);
    humidityHistory.fill((int16_t)humidity);
    pressureHistory.fill((int16_t)pressure);
  } else {
    SERIAL_PRINTLN("Failed to read sensors!");
  }

  SERIAL_PRINTLN("Thermohygrometer");
  delay(3 * 1000);
}

void loop() {
  unsigned long currentMillis = millis();
  bool needUpdate = false;

  button1.update();
  button2.update();

  if (button1.isClicked()) {
    displayFlipped = !displayFlipped;
    display.setRotation(displayFlipped ? 2 : 0);
    needUpdate = true;
  }

  if (button2.isClicked()) {
    patternIndex = (patternIndex + 1) % (sizeof(showPatternFuncs) / sizeof(showPatternFunc_t));
    needUpdate = true;
  }

  if (lastMillis == 0 || currentMillis - lastMillis >= DISPLAY_UPDATE_INTERVAL_MS) {
    float temperature, humidity, pressure;
    if (!readSensors(&temperature, &humidity, &pressure)) {
      SERIAL_PRINTLN("Failed to read sensors!");
      goto EOL;
    }

    temperatureHistory.prepend(temperature);
    humidityHistory.prepend(humidity);
    pressureHistory.prepend(pressure);

    lastMillis = currentMillis;
    needUpdate = true;
  }

  if (needUpdate) showPatternFuncs[patternIndex]();

EOL:
  delay(10);
}

void abend() {
  while (true) { delay(100); }
}

bool readSensors(float* temperature, float* humidity, float* pressure) {
  sensors_event_t temperatureEvent, humidityEvent;
  thermometer.getEvent(&humidityEvent, &temperatureEvent);
  if (isnan(temperatureEvent.temperature) || isnan(humidityEvent.relative_humidity)) {
    SERIAL_PRINTLN("Failed to read AHTX0!");
    return false;
  }
  *temperature = temperatureEvent.temperature;
  *humidity = humidityEvent.relative_humidity;

  *pressure = barometer.readPressure();
  if (isnan(*pressure)) {
    SERIAL_PRINTLN("Failed to read BMP280!");
    return false;
  }
  *pressure = *pressure / 100.0f;

  return true;
}

void showPattern1() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(DISPLAY_COLOR_WHITE);
  display.setCursor(0, 0);
  display.print(temperatureHistory.getValue(0));
  display.println("C");
  display.print(humidityHistory.getValue(0));
  display.println("%");
  display.print(pressureHistory.getValue(0));
  display.println("hPa");
  display.display();
}

void showPattern2() {
  display.clearDisplay();
  drawChart(temperatureHistory);
  drawTextWithBackground(temperatureHistory.getValue(0), "C");
  display.display();
}

void showPattern3() {
  display.clearDisplay();
  drawChart(humidityHistory);
  drawTextWithBackground(humidityHistory.getValue(0), "%");
  display.display();
}

void showPattern4() {
  display.clearDisplay();
  drawChart(pressureHistory);
  drawTextWithBackground(pressureHistory.getValue(0), "hPa");
  display.display();
}

void drawChart(History& history) {
  float minValue = history.getMinValue();
  float maxValue = history.getMaxValue();

  float tempRange = maxValue - minValue;
  if (tempRange < 0.1f) { tempRange = 0.1f; }

  for (uint8_t x = 0; x < DISPLAY_WIDTH - 1; x++) {
    float currentValue = history.getValue(DISPLAY_WIDTH - 1 - x);
    float nextValue = history.getValue(DISPLAY_WIDTH - 2 - x);

    uint8_t currentY = 17 + (uint8_t)((maxValue - currentValue) * (DISPLAY_HEIGHT - 1 - 17) / tempRange);
    uint8_t nextY = 17 + (uint8_t)((maxValue - nextValue) * (DISPLAY_HEIGHT - 1 - 17) / tempRange);

    if (currentY >= DISPLAY_HEIGHT) currentY = DISPLAY_HEIGHT - 1;
    if (nextY >= DISPLAY_HEIGHT) nextY = DISPLAY_HEIGHT - 1;

    display.drawLine(x, currentY, x + 1, nextY, DISPLAY_COLOR_WHITE);
  }
}

void drawTextWithBackground(float value, const char* unit) {
  String text = String(value) + unit;
  display.setTextColor(DISPLAY_COLOR_WHITE);
  display.setCursor(0, 0);
  display.print(value);
  display.println(unit);
}
