// #define NODEID 1

#include "K32.h"

K32* engine;

void setup() {

  engine = new K32({
    .stm32    = true,     // stm32 event listening and battery monitoring
    .leds     = true,     // dual ws2812 
    .audio    = true,     // audio engine with PCM51xx sound card
    .sampler  = true,     // media indexing to midi bank/note-xxx
    .wifi     = {         
      .ssid = "kxkm24",             // ssid (NULL to disable)
      .password = NULL,             // password (NULL if not secured)
      .ip = NULL                    // static ip (NULL to use DHCP)
    },
    .osc  = {
      .port_in  = 1818,             // osc port input (0 = disable)  // 1818
      .port_out = 1819,             // osc port output (0 = disable) // 1819
      .beatInterval     = 0,        // heartbeat interval milliseconds (0 = disable)
      .beaconInterval   = 3000      // full beacon interval milliseconds (0 = disable)
    },
    .mqtt = {
      .broker = "2.0.0.1",      //"2.0.0.1"
      .beatInterval = 0            // heartbeat interval milliseconds (0 = disable)
    }

  });

  // Settings SET
  #ifdef NODEID
    engine->settings->set("id", NODEID);
    engine->settings->set("model", 2);   // 0: proto -- 1: big -- 2: small
    engine->settings->set("channel", 15);
  #endif

}


void loop() {

  if (engine->stm32->clicked()) {

    if (engine->audio->isPlaying()) {
      engine->audio->stop();
    }
    else {
      engine->audio->loop(true);
      engine->audio->play( engine->sampler->path( engine->settings->get("id"), 16)  );
      engine->audio->volume(50);
    }

    // if (engine->audio->isPlaying() || engine->leds->isPlaying()) {
    //   engine->audio->stop();
    //   engine->leds->stop();
    // }
    // else {
    //   engine->audio->loop(true);
    //   engine->audio->play( engine->sampler->path(3, 0)  );
    //   engine->audio->volume(50);
    //   engine->leds->play("sinus");
    // }

    // engine->stm32->send(K32_stm32_api::SET_LOAD_SWITCH, 0);
    // delay(2000);
    // engine->stm32->send(K32_stm32_api::SET_LOAD_SWITCH, 1);
  }
  else if (engine->stm32->dblclicked())
    engine->stm32->reset();

  delay(500);

}
