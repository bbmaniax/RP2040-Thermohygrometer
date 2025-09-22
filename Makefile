TARGET ?= RP2040-Thermohygrometer.ino
BUILD_CONFIG ?= ./arduino-cli.yaml
FQBN ?= rp2040:rp2040:rpipico
CORES ?= rp2040:rp2040
LIBS ?= "Adafruit AHTX0" "Adafruit BMP280 Library" "Adafruit GFX Library" "Adafruit SSD1306"
DEPLOY_CMD ?= /mnt/c/Windows/System32/robocopy.exe

TEMP_DIR ?= ./tmp
BUILD_DIR ?= ./build
DEPLOY_DIR ?= D:/

.PHONY: all
all: clean install build

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEMP_DIR)
	find . -type f -exec chmod -x {} +

.PHONY: build
build: build/rp2040

.PHONY: build/rp2040
build/rp2040:
	arduino-cli --config-file $(BUILD_CONFIG) compile --fqbn $(FQBN) --export-binaries $(if $(filter-out undefined,$(origin DEBUG)),--build-property "build.extra_flags=-DDEBUG") $(TARGET)

.PHONY: install
install: core/install lib/install

.PHONY: core/install
core/install:
	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
	arduino-cli --config-file $(BUILD_CONFIG) core update-index
	arduino-cli --config-file $(BUILD_CONFIG) core install $(CORES)

.PHONY: lib/install
lib/install:
	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
	arduino-cli --config-file $(BUILD_CONFIG) lib update-index
	arduino-cli --config-file $(BUILD_CONFIG) lib install $(LIBS)

.PHONY: deploy
deploy:
	-$(DEPLOY_CMD) "$(subst /,\,$(BUILD_DIR)/$(subst :,.,$(FQBN)))" "$(DEPLOY_DIR)" *.uf2
