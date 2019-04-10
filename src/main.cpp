
#define MP_VERSION  1.0   // Refactoring


#include <Arduino.h>
#include <WiFi.h>

// ESP8266 compile dependencies
#include <HTTPClient.h>
#include <SPIFFS.h>
#include <FS.h>

// KESP32 lib
#include "K32_settings.h"
#include "K32_stm32.h"
#include "K32_leds.h"
#include "K32_audio.h"
#include "K32_samplermidi.h"

K32_settings* settings;
K32_stm32* stm32;
K32_leds* leds;
K32_audio* audio;
K32_samplermidi* sampler;

#include "kwifi.h"
#include "ksync.h"
#include "kosc.h"

void setup() {

  LOGSETUP(); // based on #define DEBUG

  // Settings config
  String keys[16] = {"id", "channel", "model"};
  settings = new K32_settings(keys);

  // Settings SET
  // settings->set("id", 1);
  // settings->set("channel", 15);
  // settings->set("model", 1);   // 0: proto -- 1: big -- 2: small

  // STM32
  stm32 = new K32_stm32();
  stm32->listen();

  // LEDS
  leds = new K32_leds();

  // AUDIO
  audio = new K32_audio();
  if(!audio->isEngineOK()) {
    LOG("Audio engine failed to start.. RESET !");
    stm32->reset();
  }

  // SAMPLER MIDI
  sampler = new K32_samplermidi();


  // WIFI init
  String hostname = "esp-" + osc_id() + "-" + osc_ch() + "-v" + String(MP_VERSION, 2);
  wifi_set_hostname(hostname, true);
  wifi_onConnect(osc_start);
  //wifi_static("192.168.0.237");

  // WIFI go
  wifi_connect("101WIFI", "101internet");
  //wifi_connect("kxkm24");

}

int testkey = 0;

void loop() {
  bool playing = audio->run();

  // AUDIO TEST
  if (!playing) {
    testkey = testkey%2 + 1;
    audio->volume(100);
    audio->play( sampler->path(0, testkey) );
  }

  // write in memory: can't be done in thread ?
  // if (osc_newChan()) {
  //   settings->set("channel", osc_chan());
  //   osc_newChan(false);
  // }
}
