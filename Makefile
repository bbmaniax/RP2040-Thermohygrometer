SKETCH ?= RP2040-Thermohygrometer.ino

.PHONY: clean
clean:
	rm -rf ./build ./tmp

.PHONY: build
build: build/uno build/nano build/promicro build/rpipico

.PHONY: build/uno
build/uno:
	arduino-cli compile --fqbn arduino:avr:uno --export-binaries $(SKETCH)

.PHONY: build/nano
build/nano:
	arduino-cli compile --fqbn arduino:avr:nano --export-binaries $(SKETCH)

.PHONY: build/promicro
build/promicro:
	arduino-cli compile --fqbn SparkFun:avr:promicro --export-binaries $(SKETCH)

.PHONY: build/rpipico
build/rpipico:
	arduino-cli compile --fqbn rp2040:rp2040:rpipico --export-binaries $(SKETCH)

.PHONY: lib/install
lib/install:
	arduino-cli lib install "Adafruit AHTX0"
	arduino-cli lib install "Adafruit BMP280 Library"
	arduino-cli lib install "Adafruit GFX Library"
	arduino-cli lib install "Adafruit SSD1306"

.PHONY: core/install
core/install:
	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
	arduino-cli config add board_manager.additional_urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
	arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json
	arduino-cli core update-index
	arduino-cli core install rp2040:rp2040
	arduino-cli core install arduino:avr
	arduino-cli core install SparkFun:avr
