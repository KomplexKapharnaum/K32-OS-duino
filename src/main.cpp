// #define NODEID 44

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
const int TimeOut = 5; // TimeOut of Leds anims in secs.  0 = infinite
const int InterruptTime = 10 ; // Time of Leds anim interrupt in secs

const int Color1[4] = {0,100,0,0};
const int Color2[4] = {100,75,0,0};
const int Color3[4] = {100,0,0,0};



void setup() {

  engine = new K32({
    .stm32    = true,     // stm32 event listening and battery monitoring
    .leds     = true,     // dual ws2812
    .audio    = false,     // audio engine with PCM51xx sound card
    .sampler  = true,     // media indexing to midi bank/note-xxx
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


   // engine->audio->loop(true);
   // engine->audio->volume(5);
   // engine->audio->play( "/robot.mp3" );
LedsAnim = engine->leds->anim("decharge");
LedsAnim->setParam(0, TimeOut);
// LedsAnim->setParam(11,41);
//
// engine->leds->play(LedsAnim);


}


void loop()
{

  if (mode == 0) // Gauge Level
  {
  /******** Routine every 5 sec ***********/
   if ((millis() - currentTime) > InterruptTime * 1000)
   {
       if(!(engine->power->charge))
       {

       /* Set Colors */
         if((engine->power->SOC < 40 ) && (engine->power->SOC >= 10 ))
         {
           LedsAnim = engine->leds->anim("decharge"); // Set anim to decharge
           LedsAnim->setParam(0, TimeOut); // Set TimeOut
           LedsAnim->setParam(3, Color2[0]);
           LedsAnim->setParam(4, Color2[1]);
           LedsAnim->setParam(5, Color2[2]);
           LedsAnim->setParam(7, Color3[0]);
           LedsAnim->setParam(8, Color3[1]);
           LedsAnim->setParam(9, Color3[2]);
         }
         else if(engine->power->SOC < 10 )
         {
           if (sinusOn)
           {
           LedsAnim = engine->leds->anim("sinus");
           LedsAnim->setParam(3, Color3[0]);
           LedsAnim->setParam(4, Color3[1]);
           LedsAnim->setParam(5, Color3[2]);
           LedsAnim->setParam(6, 0);
           sinusOn=false;
           }
           else
           {
             LedsAnim = engine->leds->anim("chaser");
             LedsAnim->setParam(3, Color3[0]);
             LedsAnim->setParam(4, Color3[1]);
             LedsAnim->setParam(5, Color3[2]);
             LedsAnim->setParam(6, 0);
             sinusOn = true;
           }
         } else // SOC > 40
         {
           LedsAnim = engine->leds->anim("decharge"); // Set anim to decharge
           LedsAnim->setParam(0, TimeOut); // Set TimeOut
           LedsAnim->setParam(3, Color1[0]);
           LedsAnim->setParam(4, Color1[1]);
           LedsAnim->setParam(5, Color1[2]);
           LedsAnim->setParam(7, Color2[0]);
           LedsAnim->setParam(8, Color2[1]);
           LedsAnim->setParam(9, Color2[2]);

         }
       }
       else
       {
         LedsAnim = engine->leds->anim("charge"); // Set anim to charge
         LedsAnim->setParam(0, TimeOut); // Set TimeOut

       /* Set Colors */
          if((engine->power->SOC < 40 )&& (engine->power->SOC >= 10 ))
          {
            LedsAnim->setParam(3, Color2[0]);
            LedsAnim->setParam(4, Color2[1]);
            LedsAnim->setParam(5, Color2[2]);
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
       }


       LedsAnim->setParam(11, engine->power->SOC); // Set State Of Charge
       engine->leds->play(LedsAnim); // Play anim
     currentTime=millis();
   }
 } else if(mode == 1) // Power 1
 {
   if (millis() - currentTime > InterruptTime * 1000)
   {
     LedsAnim = engine->leds->anim("current"); // Set anim to current
     LedsAnim->setParam(0, TimeOut); // Set TimeOut
     LedsAnim->setParam(1, Color1[0]);
     LedsAnim->setParam(2, Color1[1]);
     LedsAnim->setParam(3, Color1[2]);
     LedsAnim->setParam(5, Color2[0]);
     LedsAnim->setParam(6, Color2[1]);
     LedsAnim->setParam(7, Color2[2]);
     PowerValue = engine->power->power() ;
     LedsAnim->setParam(9, PowerValue/3000); // Set State Of Charge
     engine->leds->play(LedsAnim); // Play anim
     currentTime=millis();

   }
   if ((millis() - loopTime) > 200)
   {
     PowerValue = engine->power->power() ;
     LedsAnim->setParam(9, PowerValue/3000); // Set Power Value
   loopTime=millis();
   }
 }
 else if(mode ==2) // Power 2
 {
   if (millis() - currentTime > InterruptTime * 1000)
     {
       LedsAnim = engine->leds->anim("current2"); // Set anim to current2
       LedsAnim->setParam(0, TimeOut); // Set TimeOut
       LedsAnim->setParam(1, Color1[0]);
       LedsAnim->setParam(2, Color1[1]);
       LedsAnim->setParam(3, Color1[2]);
       LedsAnim->setParam(5, Color2[0]);
       LedsAnim->setParam(6, Color2[1]);
       LedsAnim->setParam(7, Color2[2]);
       PowerValue = engine->power->power() ;
       LedsAnim->setParam(9, PowerValue/3000); // Set State Of Charge
       engine->leds->play(LedsAnim); // Play anim
       currentTime=millis();

   }
 if ((millis() - loopTime) > 200)
   {
     PowerValue = engine->power->power() ;
     LedsAnim->setParam(9, PowerValue/3000); // Set Power Value
   loopTime=millis();
   }
 } else if(mode ==3) // Double visu
 {
   if (millis() - currentTime > InterruptTime * 1000)
     {
       LedsAnim = engine->leds->anim("double"); // Set anim to current2
       LedsAnim->setParam(0, TimeOut); // Set TimeOut
       LedsAnim->setParam(1, Color1[0]);
       LedsAnim->setParam(2, Color1[1]);
       LedsAnim->setParam(3, Color1[2]);
       LedsAnim->setParam(5, Color2[0]);
       LedsAnim->setParam(6, Color2[1]);
       LedsAnim->setParam(7, Color2[2]);
       LedsAnim->setParam(10, engine->power->SOC);

       PowerValue = engine->power->power() ;
       LedsAnim->setParam(9, PowerValue/3000); // Set State Of Charge
       engine->leds->play(LedsAnim); // Play anim
       currentTime=millis();

   }
 if ((millis() - loopTime) > 200)
   {
     PowerValue = engine->power->power() ;
     LedsAnim->setParam(9, PowerValue/3000); // Set Power Value
   loopTime=millis();
   }
 } else if (mode == 4)
 {
   if (millis() - currentTime > InterruptTime * 1000)
   {
       if (sinusOn)
       {
       LedsAnim = engine->leds->anim("sinus");
       LedsAnim->setParam(3, Color3[0]);
       LedsAnim->setParam(4, Color3[1]);
       LedsAnim->setParam(5, Color3[2]);
       LedsAnim->setParam(6, 0);
       sinusOn=false;
       }
       else
       {
         LedsAnim = engine->leds->anim("chaser");
         LedsAnim->setParam(3, Color3[0]);
         LedsAnim->setParam(4, Color3[1]);
         LedsAnim->setParam(5, Color3[2]);
         LedsAnim->setParam(6, 0);
         sinusOn = true;
       }

      engine->leds->play(LedsAnim); // Play anim
     currentTime=millis();
    }
 }



   if (engine->stm32->clicked()) {
     mode ++;
     if (mode == 5) mode = 0;




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
