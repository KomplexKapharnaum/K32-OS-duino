


#define K32_VERSION  0.9   // Refactoring

#include "K32.h"

K32* engine;

void setup() {
  engine = new K32();
}

int testkey = 0;

void loop() {

  if (engine->stm32->dblclicked()) engine->stm32->reset();


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
