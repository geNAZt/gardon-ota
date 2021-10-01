#/bin/sh

# Build SPIFFS first
pio run --target buildfs

# Build firmware
pio run

# Copy over all build artifacts
cp .pio/build/esp32dev/firmware.bin ../server/firmware/firmware.bin
cp .pio/build/esp32dev/spiffs.bin ../server/firmware/spiffs.bin