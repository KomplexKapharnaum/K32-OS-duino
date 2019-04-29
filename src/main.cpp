#define NODEID 255

#include "K32.h"

K32* engine;

void setup() {

  engine = new K32({
    .stm32    = false,     // stm32 event listening and battery monitoring
    .leds     = true,     // dual ws2812 
    .audio    = true,     // audio engine with PCM51xx sound card
    .sampler  = true,     // media indexing to midi bank/note-xxx
    .wifi     = {         
      .ssid = "interweb",             // ssid (NULL to disable)
      .password = "superspeed37",     // password (NULL if not secured)
      .ip = NULL                      // static ip (NULL to use DHCP)
    },
    .osc      = {         
      .port = 1818,          // osc port (0 = disable)
      .beat = 0,              // heartbeat interval milliseconds (0 = disable)
      .beacon = 0             // full beacon interval milliseconds (0 = disable)
    }
  });
  
  // Settings SET
  #ifdef NODEID
    engine->settings->set("id", NODEID);
  #endif
  engine->settings->set("channel", 1);
  engine->settings->set("model", 1);   // 0: proto -- 1: big -- 2: small

}

void loop() {

  if (engine->stm32->dblclicked()) engine->stm32->reset();
  delay(10);


  // AUDIO TEST
  // static int testkey = 0;
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

/*
NOTES TODO !!

- CLICKS and BLIPS audio => underflow dma zero ?
- LEDS: stop anim in progress (better time frame)

*/