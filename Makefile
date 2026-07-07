# Makefile for Arduino application projects
.DEFAULT_GOAL := all

## Project specific configurations

PROJECT ?= RP2040-Thermohygrometer
SKETCH ?= $(PROJECT).ino
LIBS ?= "Adafruit AHTX0" "Adafruit BMP280 Library" "Adafruit SSD1306" "Adafruit NeoPixel" DigitalButton
CORES ?= rp2040:rp2040

## Project specific targets

.PHONY: build
build: build/rp2040-zero

.PHONY: deploy
deploy: deploy/rp2040-zero

## Configurations

BUILD_CONFIG ?= ./arduino-cli.yaml
BUILD_DIR ?= ./build

DEPLOY_UF2_PORT ?= A:/

## Macros

define build-arduino
	arduino-cli compile \
		--fqbn $(1) \
		--export-binaries \
		$(if $(filter-out undefined,$(origin DEBUG)),--build-property "build.extra_flags=-DDEBUG") \
		$(SKETCH)
endef

define deploy-arduino
	arduino-cli upload --verbose \
		-b $(1) \
		-p $(2) \
		--input-file $(BUILD_DIR)/$(subst :,.,$(word 1,$(subst :, ,$(1))).$(word 2,$(subst :, ,$(1))).$(word 3,$(subst :, ,$(1))))/$(basename $(3)).ino.hex
endef

define deploy-arduinoasisp
	arduino-cli upload --verbose \
		-b $(1) \
		-p $(2) \
		-P arduinoasisp \
		--input-dir $(BUILD_DIR)/$(subst :,.,$(word 1,$(subst :, ,$(1))).$(word 2,$(subst :, ,$(1))).$(word 3,$(subst :, ,$(1))))
endef

define deploy-uf2
	/mnt/c/Windows/System32/robocopy.exe \
		"$(subst /,\,$(BUILD_DIR)/$(subst :,.,$(word 1,$(subst :, ,$(1))).$(word 2,$(subst :, ,$(1))).$(word 3,$(subst :, ,$(1)))))" \
		"$(DEPLOY_UF2_PORT)" $(2).ino.uf2
endef

## Targets

.PHONY: clean
clean:
	find . -type d -name "build" -exec rm -rf {} +
	find . -type f -name "*.lst" -exec rm {} +
	find . -type f -name "*.map" -exec rm {} +
	find . -type f -name "*.gcda" -exec rm {} +
	find . -type f -name "*.gcno" -exec rm {} +
	find . -type f -exec chmod -x {} +

.PHONY: all
all: clean install build

.PHONY: install
install: install/core install/lib

.PHONY: install/core
install/core:
ifeq ($(strip $(CORES)),)
	@echo "No cores defined. Skipping install/core."
else
	@echo "Installing Arduino cores..."
	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
	arduino-cli --config-file $(BUILD_CONFIG) core update-index
	arduino-cli --config-file $(BUILD_CONFIG) core install $(CORES)
	@echo ""
	@echo "Arduino cores installed."
endif

.PHONY: install/lib
install/lib:
ifeq ($(strip $(LIBS)),)
	@echo "No libraries defined. Skipping install/lib."
else
	@echo "Installing Arduino libraries..."
	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
	arduino-cli --config-file $(BUILD_CONFIG) lib update-index
	arduino-cli --config-file $(BUILD_CONFIG) lib install $(LIBS)
	@echo ""
	@echo "Arduino libraries installed."
endif

## Build targets for each board

.PHONY: build/rp2040-zero
build/rp2040-zero:
	$(call build-arduino,rp2040:rp2040:waveshare_rp2040_zero)

.PHONY: build/rpipico
build/rpipico:
	$(call build-arduino,rp2040:rp2040:rpipico)

## Deploy targets for each board

.PHONY: deploy/rp2040-zero
deploy/rp2040-zero:
	$(call deploy-uf2,rp2040:rp2040:waveshare_rp2040_zero,$(PROJECT))

.PHONY: deploy/rpipico
deploy/rpipico:
	$(call deploy-uf2,rp2040:rp2040:rpipico,$(PROJECT))
