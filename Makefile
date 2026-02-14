PROJECT ?= RP2040-Thermohygrometer
SKETCH ?= $(PROJECT).ino
SKETCHES ?= $(PROJECT).ino
LIBS ?= "Adafruit AHTX0" "Adafruit BMP280 Library" "Adafruit SSD1306" "Adafruit NeoPixel" DigitalButton
TESTS ?=
TEST_SOURCES ?=

BOARDS ?= rp2040-zero
CORES ?= rp2040:rp2040

TMP_DIR ?= ./tmp
BIN_DIR ?= ./bin
COVERAGE_DIR ?= ./coverage

BUILD_CONFIG ?= ./arduino-cli.yaml
BUILD_DIR ?= ./build

DEPLOY_DIR ?= ~/Arduino/libraries/$(PROJECT)

DEPLOY_ARDUINO_PORT_TTYUSB ?= /dev/ttyUSB0
DEPLOY_ARDUINO_PORT_TTYACM ?= /dev/ttyACM0

DEPLOY_UF2_CMD ?= /mnt/c/Windows/System32/robocopy.exe
DEPLOY_UF2_PORT ?= A:/

# This section should be appended after project-specific variable definitions
# Projects should define:
# - PROJECT, SKETCH, SKETCHES, TESTS, TEST_SOURCES
# - BOARDS, CORES, LIBS
# - TMP_DIR, BIN_DIR, COVERAGE_DIR, BUILD_CONFIG, BUILD_DIR
# - DEPLOY_DIR (for library projects only)
# - DEPLOY_ARDUINO_PORT_TTYUSB, DEPLOY_ARDUINO_PORT_TTYACM
# - DEPLOY_UF2_CMD, DEPLOY_UF2_PORT

define build-arduino
	$(foreach sketch,$(SKETCHES),arduino-cli compile \
		--library ./src \
		--fqbn $(1) \
		--export-binaries \
		$(if $(filter-out undefined,$(origin DEBUG)),--build-property "build.extra_flags=-DDEBUG") \
		$(sketch) || exit 1;)
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
	$(DEPLOY_UF2_CMD) \
		"$(subst /,\,$(BUILD_DIR)/$(subst :,.,$(word 1,$(subst :, ,$(1))).$(word 2,$(subst :, ,$(1))).$(word 3,$(subst :, ,$(1)))))" \
		"$(DEPLOY_UF2_PORT)" $(2).ino.uf2
endef

define deploy-openocd-ch32v
	$(HOME)/.arduino15/packages/ch32-riscv-arduino/tools/openocd/1.91/bin/openocd \
		-f $(HOME)/.arduino15/packages/ch32-riscv-arduino/tools/openocd/1.91/bin/wch-riscv.cfg \
		-c init -c halt -c "program {$(BUILD_DIR)/$(subst :,.,$(word 1,$(subst :, ,$(1))).$(word 2,$(subst :, ,$(1))).$(word 3,$(subst :, ,$(1))))/$(2).ino.elf} verify; reset; wlink_reset_resume; exit;"
endef

.PHONY: clean
clean:
	rm -rf $(TMP_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(COVERAGE_DIR)
	find . -type d -name "build" -exec rm -rf {} +
	find . -type f -name "*.lst" -exec rm {} +
	find . -type f -name "*.map" -exec rm {} +
	find . -type f -name "*.gcda" -exec rm {} +
	find . -type f -name "*.gcno" -exec rm {} +
	find . -type f -exec chmod -x {} +

.PHONY: all
all: clean install test coverage build

.PHONY: install
install: install/core install/lib install/tool

.PHONY: test
test: test/native

.PHONY: coverage
coverage: coverage/native

.PHONY: build
build:
	@echo "Building for all boards..."
	@for board in $(BOARDS); do \
		echo "Building for $$board..."; \
		$(MAKE) build/$$board || exit 1; \
		echo ""; \
	done
	@echo "Builds completed."

# Library-specific targets (only for library projects with DEPLOY_DIR defined)
.PHONY: deploy
deploy:
ifdef DEPLOY_DIR
	@echo "Deploying library to $(DEPLOY_DIR)..."
	mkdir -p $(DEPLOY_DIR)
	cp -r README.md keywords.txt library.properties ./src $(DEPLOY_DIR)
	@echo ""
	@echo "Library deployed to $(DEPLOY_DIR)."
else
	@echo "DEPLOY_DIR not defined. Skipping deploy."
endif

.PHONY: undeploy
undeploy:
ifdef DEPLOY_DIR
	@echo "Removing deployed library from $(DEPLOY_DIR)..."
	rm -fr $(DEPLOY_DIR)
	@echo ""
	@echo "Deployed library removed from $(DEPLOY_DIR)."
else
	@echo "DEPLOY_DIR not defined. Skipping undeploy."
endif

.PHONY: run
run:
	@echo "Run target not implemented for this project."

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

.PHONY: install/tool
install/tool:
	@echo "Installing required tools..."
	sudo apt-get update
	sudo apt-get install -y build-essential lcov libgtest-dev
	@echo ""
	@echo "Required tools installed."

.PHONY: test/native
test/native:
ifeq ($(strip $(TESTS)),)
	@echo "No tests defined. Skipping test/native."
else
	@mkdir -p $(BIN_DIR)
	@echo "Building and running tests..."
	$(foreach test,$(TESTS),$(call build-test,$(test));)
	@echo ""
	@echo "Running tests..."
	$(foreach test,$(TESTS),$(call run-test,$(test));)
	@echo ""
	@echo "All tests completed."
endif

.PHONY: coverage/native
coverage/native:
ifeq ($(strip $(TESTS)),)
	@echo "No tests defined. Skipping coverage/native."
else
	@echo "Generating coverage report..."
	@rm -rf $(COVERAGE_DIR)
	@mkdir -p $(COVERAGE_DIR)
	@lcov --capture --directory . --output-file $(COVERAGE_DIR)/coverage.info --branch-coverage --ignore-errors mismatch 2>/dev/null
	@lcov --remove $(COVERAGE_DIR)/coverage.info '*/test/*' '/usr/include/*' --output-file $(COVERAGE_DIR)/coverage.info --branch-coverage --ignore-errors unused 2>&1 | grep -v "^Excluding"
	@genhtml $(COVERAGE_DIR)/coverage.info --output-directory $(COVERAGE_DIR)/html --branch-coverage 2>&1 | grep -E "(^Found|^Processing|^Overall)"
	@echo ""
	@echo "Coverage report generated at $(COVERAGE_DIR)/html/index.html"
endif

.PHONY: build/uno
build/uno:
	$(call build-arduino,arduino:avr:uno)

.PHONY: build/nano
build/nano:
	$(call build-arduino,arduino:avr:nano:cpu=atmega328)

.PHONY: build/nano-old
build/nano-old: build/nano

.PHONY: build/promini
build/promini:
	$(call build-arduino,arduino:avr:pro:cpu=16MHzatmega328)

.PHONY: build/leonardo
build/leonardo:
	$(call build-arduino,arduino:avr:leonardo)

.PHONY: build/promicro
build/promicro:
	$(call build-arduino,SparkFun:avr:promicro:cpu=16MHzatmega32U4)

.PHONY: build/attiny85
build/attiny85:
	$(call build-arduino,ATTinyCore:avr:attinyx5:chip=85)

.PHONY: build/attiny13
build/attiny13:
	$(call build-arduino,MicroCore:avr:13)

.PHONY: build/lgt8f328p-uno
build/lgt8f328p-uno:
	$(call build-arduino,lgt8fx:avr:328)

.PHONY: build/lgt8f328p-nano
build/lgt8f328p-nano: build/lgt8f328p-uno

.PHONY: build/unor4-minima
build/unor4-minima:
	$(call build-arduino,arduino:renesas_uno:minima)

.PHONY: build/unor4-wifi
build/unor4-wifi:
	$(call build-arduino,arduino:renesas_uno:unor4wifi)

.PHONY: build/rpipico
build/rpipico:
	$(call build-arduino,rp2040:rp2040:rpipico)

.PHONY: build/rp2040-zero
build/rp2040-zero:
	$(call build-arduino,rp2040:rp2040:waveshare_rp2040_zero)

.PHONY: build/rp2040-promicro
build/rp2040-promicro:
	$(call build-arduino,rp2040:rp2040:sparkfun_promicrorp2040)

.PHONY: build/esp32c3-supermini
build/esp32c3-supermini:
	$(call build-arduino,esp32:esp32:nologo_esp32c3_super_mini)

.PHONY: build/xiao-esp32c3
build/xiao-esp32c3:
	$(call build-arduino,esp32:esp32:XIAO_ESP32C3)

.PHONY: build/xiao-esp32c6
build/xiao-esp32c6:
	$(call build-arduino,esp32:esp32:XIAO_ESP32C6)

.PHONY: build/uaip-promicro
build/uaip-promicro:
	$(call build-arduino,UIAP:ch32v:CH32V00x_EVT)

.PHONY: build/ch32v003
build/ch32v003:
	$(foreach sketch,$(SKETCHES),arduino-cli compile \
		--library ./src \
		--fqbn ch32-riscv-arduino:ch32riscv:CH32V003_EVT \
		--export-binaries \
		--build-property "build.extra_flags=-flto" \
		--build-property "compiler.c.extra_flags=-flto" \
		--build-property "compiler.cpp.extra_flags=-flto" \
		$(sketch) || exit 1;)

.PHONY: deploy/uno
deploy/uno:
	$(call deploy-arduino,arduino:avr:uno,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/nano
deploy/nano:
	$(call deploy-arduino,arduino:avr:nano:cpu=atmega328,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/nano-old
deploy/nano-old:
	$(call deploy-arduino,arduino:avr:nano:cpu=atmega328old,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/promini
deploy/promini:
	$(call deploy-arduino,arduino:avr:pro:cpu=16MHzatmega328,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/leonardo
deploy/leonardo:
	$(call deploy-arduino,arduino:avr:leonardo,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/promicro
deploy/promicro:
	$(call deploy-arduino,SparkFun:avr:promicro:cpu=16MHzatmega32U4,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/attiny85
deploy/attiny85:
	$(call deploy-arduinoasisp,ATTinyCore:avr:attinyx5:chip=85,$(DEPLOY_ARDUINO_PORT_TTYUSB))

.PHONY: deploy/attiny13
deploy/attiny13:
	$(call deploy-arduinoasisp,MicroCore:avr:13,$(DEPLOY_ARDUINO_PORT_TTYUSB))

.PHONY: deploy/lgt8f328p-uno
deploy/lgt8f328p-uno:
	$(call deploy-arduino,lgt8fx:avr:328,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/lgt8f328p-nano
deploy/lgt8f328p-nano:
	$(call deploy-arduino,lgt8fx:avr:328,$(DEPLOY_ARDUINO_PORT_TTYUSB),$(SKETCH))

.PHONY: deploy/unor4-minima
deploy/unor4-minima:
	$(call deploy-arduino,arduino:renesas_uno:minima,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/unor4-wifi
deploy/unor4-wifi:
	$(call deploy-arduino,arduino:renesas_uno:unor4wifi,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/rpipico
deploy/rpipico:
	$(call deploy-uf2,rp2040:rp2040:rpipico,$(PROJECT))

.PHONY: deploy/rp2040-zero
deploy/rp2040-zero:
	$(call deploy-uf2,rp2040:rp2040:waveshare_rp2040_zero,$(PROJECT))

.PHONY: deploy/rp2040-promicro
deploy/rp2040-promicro:
	$(call deploy-uf2,rp2040:rp2040:sparkfun_promicrorp2040,$(PROJECT))

.PHONY: deploy/esp32c3-supermini
deploy/esp32c3-supermini:
	$(call deploy-arduino,esp32:esp32:nologo_esp32c3_super_mini,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/xiao-esp32c3
deploy/xiao-esp32c3:
	$(call deploy-arduino,esp32:esp32:XIAO_ESP32C3,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/xiao-esp32c6
deploy/xiao-esp32c6:
	$(call deploy-arduino,esp32:esp32:XIAO_ESP32C6,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/uaip-promicro
deploy/uaip-promicro:
	$(call deploy-arduino,UIAP:ch32v:CH32V00x_EVT,$(DEPLOY_ARDUINO_PORT_TTYACM),$(SKETCH))

.PHONY: deploy/ch32v003
deploy/ch32v003:
	$(call deploy-openocd-ch32v,ch32-riscv-arduino:ch32riscv:CH32V003_EVT,$(basename $(SKETCH)))
