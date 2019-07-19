# K32-OS-duino

Operating System embedded in KXKM ESP32 boards.

Created with PlatformIO (Atom) with Arduino framework.

To start working with this project:
- Install Atom + PlatformIO
- git clone --recurse-submodules https://github.com/KomplexKapharnaum/K32-OS-duino.git
- (if you cloned without --recurse: git submodule update --init --recursive)
- Open Project with PlatformIO
- Build and Flash !


CHALON:


BUG:
- STM32: btn bug without serial...
- MQTT: can't publish..

MAJOR:
- MONITOR system 
- ARTNET: node RIRI
- OSC: Burst (msg id)
- OSC: Beacon: JSON
- MIDI: OSC Burst (instead of MQTT)
- Audio sleep: replace delay(10) with mutex
- K32 Bridge: choose iface + mode (MQTT / OSC / OSC Burst)
- MQTT: system settings
- SYS: hardware version (+pinout) from stm32
- SYS: id from stm32 

MINOR:
- ARTNET: to/from DMX
- OSC: LOG args on receive
- OSC: channel change
- MQTT: leds dispatch (not done yet)
- MQTT: BROKER with mDNS declaration (auto-discovery)
- Unbind sd (init and check) from audio engine


FUTURE:
- OSC: beat Multicast
- Better conf system (with webpage ? and AP ?)
- ESP-IDF !!



