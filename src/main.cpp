


#define K32_VERSION  0.9   // Refactoring

#include "K32.h"

K32* engine;

void setup() {
  
  engine = new K32({
    .stm32    = false,     // stm32 event listening and battery monitoring
    .leds     = true,     // dual ws2812 
    .audio    = true,     // audio engine with PCM51xx sound card
    .sampler  = true,     // media indexing to midi bank/note-xxx
    .osc      = 1818,     // osc port (0 = disable)
    .wifi     = {         // wifi settings ({} to disable)
      .ssid = "interweb",             // ssid (NULL to disable)
      .password = "superspeed37",     // password (NULL if not secured)
      .ip = NULL                      // static ip (NULL to use DHCP)
    }
  });

}

void loop() {

  if (engine->stm32->dblclicked()) engine->stm32->reset();


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
