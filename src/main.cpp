//#define NODEID (108)

#include "K32.h"


#define HW_REVISION 2




K32* engine;
unsigned long currentTime=millis();
unsigned long loopTime = millis();
bool previousChargeState = true; // t
K32_leds_anim* LedsAnim ;
// Current sensor on GPIO 35
const int CurrentSensor = 35;

// variable for storing power value value
int PowerValue = 0;
int RefValue = 0;
int mode = 0;
bool sinusOn = true;
int TimeOutDech = 0 ; // Nb of cycle TimeOut of Leds anims in secs.  0 = infinite
int TimeOutCharge = 0 ;
int InterruptTime = 3 ; // Time of Leds anim interrupt in secs

const int Color1[4] = {0,101,0,0}; /* Green */
const int Color2[4] = {100,75,0,0}; /* Orange */
const int Color3[4] = {100,0,0,0}; /* Red*/



void setup() {

  engine = new K32({
    .stm32    = true,     // stm32 event listening and battery monitoring
    .leds     = true,     // dual ws2812
    .audio    = false,     // audio engine with PCM51xx sound card
    .sampler  = false,     // media indexing to midi bank/note-xxx
    .power    = true,    // power monitoring
    .wifi     = {
      .ssid = "kxkm-wifi",             // ssid (NULL to disable)
      .password = "KOMPLEXKAPHARNAUM",             // password (NULL if not secured)
      .ip = NULL                    // static ip (NULL to use DHCP)
    },
    .osc  = {
      .port_in  = 1818,             // osc port input (0 = disable)
      .port_out = 1819,             // osc port output (0 = disable)
      .beatInterval     = 1000,        // heartbeat interval milliseconds (0 = disable)
      .beaconInterval   = 5000      // full beacon interval milliseconds (0 = disable)
    }
  });

  // Settings SET
  #ifdef NODEID
    engine->settings->set("id", NODEID);
    engine->settings->set("channel", 15);
  #endif
    engine->settings->set("model", 2);   // 0: proto -- 1: big -- 2: small

   if (engine->power)
    engine->power->start();

    engine->stm32->custom(3500*7, 3650*7, 3700*7, 3750*7, 3825*7, 3950*7, 4200*7);



LedsAnim = engine->leds->anim("charge");
LedsAnim->setParam(0, TimeOutCharge );
engine->leds->play(LedsAnim); // Play anim




}


void loop()
{

  /******** Routine every 5 sec ***********/
   if ((millis() - currentTime) > InterruptTime * 1000)
   {

        // LedsAnim = engine->leds->anim("charge"); // Set anim to charge
         //LedsAnim->setParam(0, TimeOutCharge); // Set TimeOut

       /* Set Colors */
          if((engine->power->SOC < 40 )&& (engine->power->SOC >= 10 ))
          {
            LedsAnim->setParam(3, Color1[0]);
            LedsAnim->setParam(4, Color1[1]);
            LedsAnim->setParam(5, Color1[2]);
            LedsAnim->setParam(7, Color3[0]);
            LedsAnim->setParam(8, Color3[1]);
            LedsAnim->setParam(9, Color3[2]);
          }
          else if(engine->power->SOC < 10 )
          {
            LedsAnim->setParam(3, Color3[0]);
            LedsAnim->setParam(4, Color3[1]);
            LedsAnim->setParam(5, Color3[2]);
            LedsAnim->setParam(7, 0);
            LedsAnim->setParam(8, 0);
            LedsAnim->setParam(9, 0);
          } else
          {
            LedsAnim->setParam(3, Color1[0]);
            LedsAnim->setParam(4, Color1[1]);
            LedsAnim->setParam(5, Color1[2]);
            LedsAnim->setParam(7, Color2[0]);
            LedsAnim->setParam(8, Color2[1]);
            LedsAnim->setParam(9, Color2[2]);
          }


       LedsAnim->setParam(11, engine->power->SOC); // Set State Of Charge
       PowerValue = engine->power->power() ;
       LedsAnim->setParam(12, abs(PowerValue)/1000); // Set Power
       LOG(PowerValue / 1000);
       // engine->leds->play(LedsAnim); // Play anim
     currentTime=millis();
   }



   if (engine->stm32->clicked()) {

     // mode ++;
     // if (mode == 5) mode = 0;
     //engine->power->set_demo();
     //TimeOut = 0; // TimeOut of Leds anims in secs.  0 = infinite
     //InterruptTime = 5 ; // Time of Leds anim interrupt in secs




   }
   else if (engine->stm32->dblclicked())
   {

     uint8_t led_RSSI[6];
     uint8_t led_off[6] = {0, 0, 0, 0, 4, 0};

     engine->wifi->getWiFiLevel(led_RSSI);
     engine->stm32->blink(led_RSSI, 1000);

 }

  // LOGF("Heap: %d \n", xPortGetFreeHeapSize());
}
