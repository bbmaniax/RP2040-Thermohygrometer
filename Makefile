SKETCH ?= RP2040-Thermohygrometer.ino
BUILD_CONFIG ?= ./arduino-cli.yaml
FQBN ?= rp2040:rp2040:rpipico
CORES ?= rp2040:rp2040
LIBS ?= "Adafruit AHTX0" "Adafruit BMP280 Library" "Adafruit GFX Library" "Adafruit SSD1306"

BUILD_DIR ?= ./build
TEMP_DIR ?= ./tmp

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEMP_DIR)

.PHONY: build
build:
	arduino-cli --config-file $(BUILD_CONFIG) compile --fqbn $(FQBN) --export-binaries $(if $(filter-out undefined,$(origin DEBUG)),--build-property "build.extra_flags=-DDEBUG") $(SKETCH)

.PHONY: install
install: core/install lib/install

.PHONY: core/install
core/install:
	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
	arduino-cli --config-file $(BUILD_CONFIG) core update-index
	arduino-cli --config-file $(BUILD_CONFIG) core install $(CORES)

.PHONY: lib/install
lib/install:
	arduino-cli --config-file $(BUILD_CONFIG) lib install $(LIBS)
