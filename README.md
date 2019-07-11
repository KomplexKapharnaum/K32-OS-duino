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
- AUDIO: CLICKS and BLIPS audio


MINOR:
- ARTNET: to/from DMX
- OSC: LOG args on receive
- OSC: channel change
- Unbind sd (init and check) from audio engine
- SYS: hardware version (+pinout) from stm32
- SYS: id from stm32 

MAJOR:
- ARTNET: node
- MQTT: interface
- OSC: Burst (msg id)
- OSC: Beacon: JSON
- MIDI: Standalone bridge + MQTT + OSC Burst
- Monitor system 
- Audio sleep: replace delay(10) with mutex


FUTURE:
- OSC: beat Multicast
- Better conf system (with webpage ? and AP ?)


MQTT:
- BROKER raspi (mosquitto) // mDNS declaration
- MIDI bridge: virtual midi OUT // discover (mDNS) + connect MQTT broker // publish on [midi/c16/noteon]
- ESP MQTT client: discover (mDNS) + connect MQTT broker
    - audio/midi: subscribe to [midi/c16/#] // bind to audio player
    - monitoring: publish on [status/esp/32]

