#define K32_SET_NODEID        1   // board unique id    (necessary one time only)
#define K32_SET_HWREVISION    2   // board HW revision  (necessary one time only)


#include "K32.h"
K32* k32;


void setup() {
  
  k32 = new K32();

  k32->init_stm32();
  k32->init_audio();
  k32->init_light();

  // WIFI
  k32->init_wifi("classic");
  k32->wifi->connect("interweb", "superspeed37");

  // Start OSC
  k32->init_osc({
      .port_in  = 1818,             // osc port input (0 = disable)  // 1818
      .port_out = 1819,             // osc port output (0 = disable) // 1819
      .beatInterval     = 0,        // heartbeat interval milliseconds (0 = disable)
      .beaconInterval   = 3000      // full beacon interval milliseconds (0 = disable)
    });

  // Start MQTT
  k32->init_mqtt({
      .broker  = "192.168.0.70",         // MQTT broker
      .beatInterval     = 0,             // heartbeat interval milliseconds (0 = disable)
      .beaconInterval   = 3000           // full beacon interval milliseconds (0 = disable)
    });

}


void loop() {

  // if (engine->stm32->clicked()) {

  //   if (engine->audio->isPlaying()) {
  //     engine->audio->stop();
  //   }
  //   else {
  //     engine->audio->loop(true);
  //     engine->audio->play( engine->sampler->path( engine->settings->get("channel"), 0)  );
  //     engine->audio->volume(50);
  //   }

  // //   // if (engine->audio->isPlaying() || engine->leds->isPlaying()) {
  // //   //   engine->audio->stop();
  // //   //   engine->leds->stop();
  // //   // }
  // //   // else {
  // //   //   engine->audio->loop(true);
  // //   //   engine->audio->play( engine->sampler->path(3, 0)  );
  // //   //   engine->audio->volume(50);
  // //   //   engine->leds->play("sinus");
  // //   // }

  // //   // engine->stm32->send(K32_stm32_api::SET_LOAD_SWITCH, 0);
  // //   // delay(2000);
  // //   // engine->stm32->send(K32_stm32_api::SET_LOAD_SWITCH, 1);
  // }

  // if (engine->stm32->dblclicked())
  //   engine->audio->stop();

  delay(500);

  // Serial.println(ESP.getFreeHeap());

}
