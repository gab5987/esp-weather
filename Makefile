cc := pio

upload:
	pio run -t upload

listen:
	 minicom -D /dev/ttyUSB0

build:
	pio run
