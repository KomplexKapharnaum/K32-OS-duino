
#define K32_VERSION  0.9   // Refactoring


#include <Arduino.h>
#include <WiFi.h>

// ESP8266 compile dependencies
#include <HTTPClient.h>
#include <SPIFFS.h>
#include <FS.h>

// KESP32 lib
#include "K32_settings.h"
#include "K32_stm32.h"
#include "K32_wifi.h"
#include "K32_osc.h"
#include "K32_leds.h"
#include "K32_leds_anims.h"
#include "K32_audio.h"
#include "K32_samplermidi.h"

K32_settings* settings;
K32_stm32* stm32;
K32_wifi* wifi;
K32_osc* osc;
K32_leds* leds;
K32_audio* audio;
K32_samplermidi* sampler;

// #include "ksync.h"
// #include "kosc.h"

void setup() {

  LOGSETUP(); // based on #define DEBUG

  // Settings config
  String keys[16] = {"id", "channel", "model"};
  settings = new K32_settings(keys);

  // Settings SET
  settings->set("id", 32);
  settings->set("channel", 15);
  settings->set("model", 1);   // 0: proto -- 1: big -- 2: small

  // STM32
  stm32 = new K32_stm32();
  // stm32->listen(true, true);

  // AUDIO
  audio = new K32_audio();
  if(!audio->isEngineOK()) {
    LOG("Audio engine failed to start.. RESET !");
    stm32->reset();
  }
  // delay(2000);

  // LEDS
  leds = new K32_leds();
  leds->play( K32_leds_anims::test );
  leds->play( K32_leds_anims::sinus );

  // SAMPLER MIDI
  sampler = new K32_samplermidi();

  // WIFI init
  wifi = new K32_wifi( "esp-" + String(settings->get("id")) + "-v" + String(K32_VERSION, 2) );
  //wifi->static("192.168.0.237");
  wifi->connect("kxkm-wifi", "KOMPLEXKAPHARNAUM");
  // if (!wifi->wait(10)) {
  //   stm32->reset();
  // }

  // OSC init
  osc = new K32_osc(3737, 4037);
  
}

int testkey = 0;

void loop() {

  if (stm32->dblclicked()) stm32->reset();


  // leds->play( K32_leds_anims::sinus );
  // delay(4000);

  // bool playing = audio->run();

  // AUDIO TEST
  // if (!playing) {
  //   testkey = testkey%2 + 1;
  //   audio->volume(100);
  //   audio->play( sampler->path(0, testkey) );
  // }

  // write in memory: can't be done in thread ?
  // if (osc_newChan()) {
  //   settings->set("channel", osc_chan());
  //   osc_newChan(false);
  // }
}
