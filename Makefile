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

## Configrations

TMP_DIR ?= ./tmp
BIN_DIR ?= ./bin

BUILD_CONFIG ?= ./arduino-cli.yaml
BUILD_DIR ?= ./build

DEPLOY_ARDUINO_PORT_TTYUSB ?= /dev/ttyUSB0
DEPLOY_ARDUINO_PORT_TTYACM ?= /dev/ttyACM0

DEPLOY_UF2_PORT ?= A:/

## Macros

define build-arduino
	arduino-cli compile \
		--library ./src \
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

define deploy-ch32v
	arduino-cli upload --verbose \
		-b $(1) \
		--input-file $(BUILD_DIR)/$(subst :,.,$(word 1,$(subst :, ,$(1))).$(word 2,$(subst :, ,$(1))).$(word 3,$(subst :, ,$(1))))/$(2).ino.elf
endef

## Targets

.PHONY: clean
clean:
	rm -rf $(TMP_DIR)
	rm -rf $(BIN_DIR)
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

.PHONY: build/attiny13
build/attiny13:
	$(call build-arduino,MicroCore:avr:13)

.PHONY: build/attiny45
build/attiny45:
	$(call build-arduino,ATTinyCore:avr:attinyx5:chip=45)

.PHONY: build/attiny85
build/attiny85:
	$(call build-arduino,ATTinyCore:avr:attinyx5:chip=85)

.PHONY: build/ch32v003
build/ch32v003:
	$(call build-arduino,ch32-riscv-arduino:ch32riscv:CH32V003_EVT:opt=oslto)

.PHONY: build/esp32c3-supermini
build/esp32c3-supermini:
	$(call build-arduino,esp32:esp32:nologo_esp32c3_super_mini)

.PHONY: build/leonardo
build/leonardo:
	$(call build-arduino,arduino:avr:leonardo)

.PHONY: build/lgt8f328p-nano
build/lgt8f328p-nano: build/lgt8f328p-uno

.PHONY: build/lgt8f328p-promini
build/lgt8f328p-promini: build/lgt8f328p-uno

.PHONY: build/lgt8f328p-uno
build/lgt8f328p-uno:
	$(call build-arduino,lgt8fx:avr:328)

.PHONY: build/nano
build/nano:
	$(call build-arduino,arduino:avr:nano:cpu=atmega328)

.PHONY: build/nano-old
build/nano-old: build/nano

.PHONY: build/promicro
build/promicro:
	$(call build-arduino,SparkFun:avr:promicro:cpu=16MHzatmega32U4)

.PHONY: build/promini
build/promini:
	$(call build-arduino,arduino:avr:pro:cpu=16MHzatmega328)

.PHONY: build/rp2040-promicro
build/rp2040-promicro:
	$(call build-arduino,rp2040:rp2040:sparkfun_promicrorp2040)

.PHONY: build/rp2040-zero
build/rp2040-zero:
	$(call build-arduino,rp2040:rp2040:waveshare_rp2040_zero)

.PHONY: build/rpipico
build/rpipico:
	$(call build-arduino,rp2040:rp2040:rpipico)

.PHONY: build/uaip-promicro
build/uaip-promicro:
	$(call build-arduino,UIAP:ch32v:CH32V00x_EVT)

.PHONY: build/uno
build/uno:
	$(call build-arduino,arduino:avr:uno)

.PHONY: build/unor4-minima
build/unor4-minima:
	$(call build-arduino,arduino:renesas_uno:minima)

.PHONY: build/unor4-wifi
build/unor4-wifi:
	$(call build-arduino,arduino:renesas_uno:unor4wifi)

.PHONY: build/xiao-esp32c3
build/xiao-esp32c3:
	$(call build-arduino,esp32:esp32:XIAO_ESP32C3)

.PHONY: build/xiao-esp32c6
build/xiao-esp32c6:
	$(call build-arduino,esp32:esp32:XIAO_ESP32C6)

## Deploy targets for each board

.PHONY: deploy/attiny13
deploy/attiny13:
	$(call deploy-arduinoasisp,MicroCore:avr:13,$(DEPLOY_ARDUINO_PORT_TTYUSB))

.PHONY: deploy/attiny45
deploy/attiny45:
	$(call deploy-arduinoasisp,ATTinyCore:avr:attinyx5:chip=45,$(DEPLOY_ARDUINO_PORT_TTYUSB))

.PHONY: deploy/attiny85
deploy/attiny85:
	$(call deploy-arduinoasisp,ATTinyCore:avr:attinyx5:chip=85,$(DEPLOY_ARDUINO_PORT_TTYUSB))

.PHONY: deploy/ch32v003
deploy/ch32v003:
	$(call deploy-ch32v,ch32-riscv-arduino:ch32riscv:CH32V003_EVT,$(basename $(SKETCH)))

.PHONY: deploy/esp32c3-supermini
deploy/esp32c3-supermini:
	$(call deploy-arduino,esp32:esp32:nologo_esp32c3_super_mini,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/leonardo
deploy/leonardo:
	$(call deploy-arduino,arduino:avr:leonardo,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/lgt8f328p-nano
deploy/lgt8f328p-nano:
	$(call deploy-arduino,lgt8fx:avr:328,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/lgt8f328p-promini
deploy/lgt8f328p-promini:
	$(call deploy-arduino,lgt8fx:avr:328,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/lgt8f328p-uno
deploy/lgt8f328p-uno:
	$(call deploy-arduino,lgt8fx:avr:328,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/nano
deploy/nano:
	$(call deploy-arduino,arduino:avr:nano:cpu=atmega328,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/nano-old
deploy/nano-old:
	$(call deploy-arduino,arduino:avr:nano:cpu=atmega328old,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/promicro
deploy/promicro:
	$(call deploy-arduino,SparkFun:avr:promicro:cpu=16MHzatmega32U4,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/promini
deploy/promini:
	$(call deploy-arduino,arduino:avr:pro:cpu=16MHzatmega328,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/rp2040-promicro
deploy/rp2040-promicro:
	$(call deploy-uf2,rp2040:rp2040:sparkfun_promicrorp2040,$(PROJECT))

.PHONY: deploy/rp2040-zero
deploy/rp2040-zero:
	$(call deploy-uf2,rp2040:rp2040:waveshare_rp2040_zero,$(PROJECT))

.PHONY: deploy/rpipico
deploy/rpipico:
	$(call deploy-uf2,rp2040:rp2040:rpipico,$(PROJECT))

.PHONY: deploy/uaip-promicro
deploy/uaip-promicro:
	$(call deploy-arduino,UIAP:ch32v:CH32V00x_EVT,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/uno
deploy/uno:
	$(call deploy-arduino,arduino:avr:uno,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/unor4-minima
deploy/unor4-minima:
	$(call deploy-arduino,arduino:renesas_uno:minima,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/unor4-wifi
deploy/unor4-wifi:
	$(call deploy-arduino,arduino:renesas_uno:unor4wifi,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/xiao-esp32c3
deploy/xiao-esp32c3:
	$(call deploy-arduino,esp32:esp32:XIAO_ESP32C3,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/xiao-esp32c6
deploy/xiao-esp32c6:
	$(call deploy-arduino,esp32:esp32:XIAO_ESP32C6,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))
