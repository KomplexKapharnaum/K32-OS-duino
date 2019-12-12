// #define NODEID 44

#include "K32.h"


#define HW_REVISION 2




/*******************************************************************************
* Function Name  : void base_draw(void)
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void base_draw(void)
{
  int i, j;

  /*
  draw pixel
  */
  epd_clear();
  for (j = 0; j < 600; j += 50)
  {
    for (i = 0; i < 800; i += 50)
    {
      epd_draw_pixel(i, j);
      epd_draw_pixel(i, j + 1);
      epd_draw_pixel(i + 1, j);
      epd_draw_pixel(i + 1, j + 1);
    }
  }
  epd_udpate();
  delay(3000);

  /*
  draw line
  */
  epd_clear();
  for (i = 0; i < 800; i += 100)
  {
    epd_draw_line(0, 0, i, 599);
    epd_draw_line(799, 0, i, 599);
  }
  epd_udpate();
  delay(3000);

  /*
  fill rect
  */
  epd_clear();
  epd_set_color(BLACK, WHITE);
  epd_fill_rect(10, 10, 100, 100);

  epd_set_color(DARK_GRAY, WHITE);
  epd_fill_rect(110, 10, 200, 100);

  epd_set_color(GRAY, WHITE);
  epd_fill_rect(210, 10, 300, 100);

  epd_udpate();
  delay(3000);

  /*
  draw circle
  */
  epd_set_color(BLACK, WHITE);
  epd_clear();
  for (i = 0; i < 300; i += 40)
  {
    epd_draw_circle(399, 299, i);
  }
  epd_udpate();
  delay(3000);

  /*
  fill circle
  */
  epd_clear();
  for (j = 0; j < 6; j++)
  {
    for (i = 0; i < 8; i++)
    {
      epd_fill_circle(50 + i * 100, 50 + j * 100, 50);
    }
  }
  epd_udpate();
  delay(3000);

  /*
  draw triangle
  */
  epd_clear();
  for (i = 1; i < 5; i++)
  {
    epd_draw_triangle(399, 249 - i * 50, 349 - i * 50, 349 + i * 50, 449 + i * 50, 349 + i * 50);
  }
  epd_udpate();
  delay(3000);
}

void draw_text_demo(void)
{
  char buff[] = {'G', 'B', 'K', '3', '2', ':', ' ', 0xc4, 0xe3, 0xba, 0xc3, 0xca, 0xc0, 0xbd, 0xe7, 0};
  epd_set_color(BLACK, WHITE);
  epd_clear();
  epd_set_ch_font(GBK32);
  epd_set_en_font(ASCII32);
  epd_disp_string(buff, 0, 50);
  epd_disp_string("ASCII32: Hello, World!", 0, 300);

  epd_set_ch_font(GBK48);
  epd_set_en_font(ASCII48);
  buff[3] = '4';
  buff[4] = '8';
  epd_disp_string(buff, 0, 100);
  epd_disp_string("ASCII48: Hello, World!", 0, 350);

  epd_set_ch_font(GBK64);
  epd_set_en_font(ASCII64);
  buff[3] = '6';
  buff[4] = '4';
  epd_disp_string(buff, 0, 160);
  epd_disp_string("ASCII64: Hello, World!", 0, 450);


  epd_udpate();
  delay(3000);
}

void draw_bitmap_demo(void)
{
  epd_clear();
  epd_disp_bitmap("i1.BMP", 0, 0);
  epd_udpate();
  delay(3000);

  epd_clear();
  epd_disp_bitmap("i2.BMP", 0, 0);
  epd_udpate();
}



K32* engine;
unsigned long currentTime=millis();
bool previousChargeState = true; // t
K32_leds_anim* LedsAnim ;


void setup() {

  engine = new K32({
    .stm32    = true,     // stm32 event listening and battery monitoring
    .leds     = true,     // dual ws2812
    .audio    = true,     // audio engine with PCM51xx sound card
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
   engine->audio->volume(10);


   // engine->audio->loop(true);
   // engine->audio->volume(5);
   // engine->audio->play( "/robot.mp3" );
   // engine->leds->play("sinus");




   //swSer.begin(19200);
   //Serial1.begin(19200, SERIAL_8N1, 15, 13);
  // Serial2.begin(19200);
   // Serial2.setTimeout(5000);
   //Serial1.begin(19200, SERIAL_8N1, 32, 33);



}


void loop()
{
  /******** Routine every 2 sec ***********/
   if ((millis() - currentTime) > 2000)
   {
      currentTime=millis();

     if(!(engine->power->charge))
     {
       if (previousChargeState)
        {
          engine->audio->stop();
          engine->audio->loop(false);
          engine->audio->play( "/decharge1.mp3" );
          engine->leds->stop();
          previousChargeState=false;
          LedsAnim = engine->leds->anim("decharge");
          engine->leds->play(LedsAnim);
        }

       if(engine->power->SOC < 40 )
       {
         LedsAnim->setParam(3, 255);
         LedsAnim->setParam(4, 165);
         LedsAnim->setParam(5, 0);
         LedsAnim->setParam(7, 200);
         LedsAnim->setParam(8, 0);
         LedsAnim->setParam(9, 0);
       }
       else if(engine->power->SOC < 10 )
       {
         // engine->leds->stop();
         // LedsAnim = engine->leds->anim("sinus");
         // engine->leds->play(LedsAnim);
         // engine->audio->play( "/decharge1.mp3" );
       } else
       {
         LedsAnim->setParam(3, 0);
         LedsAnim->setParam(4, 100);
         LedsAnim->setParam(5, 0);
         LedsAnim->setParam(7, 255);
         LedsAnim->setParam(8, 165);
         LedsAnim->setParam(9, 0);
       }
       LedsAnim->setParam(11, engine->power->SOC);

     }
     else
     {
       if (!previousChargeState )
        {
          engine->audio->loop(true);
          engine->audio->play( "/robot.mp3" );
          previousChargeState=true;
          LedsAnim = engine->leds->anim("charge");
          LedsAnim->setParam(11, engine->power->SOC);
          engine->leds->play(LedsAnim);
        }

        if(engine->power->SOC < 40 )
        {
          LedsAnim->setParam(3, 255);
          LedsAnim->setParam(4, 165);
          LedsAnim->setParam(5, 0);
          LedsAnim->setParam(7, 200);
          LedsAnim->setParam(8, 0);
          LedsAnim->setParam(9, 0);
        }
        else if(engine->power->SOC < 10 )
        {
          LedsAnim->setParam(3, 200);
          LedsAnim->setParam(4, 0);
          LedsAnim->setParam(5, 0);
          LedsAnim->setParam(7, 0);
          LedsAnim->setParam(8, 0);
          LedsAnim->setParam(9, 0);
        } else
        {
          LedsAnim->setParam(3, 0);
          LedsAnim->setParam(4, 100);
          LedsAnim->setParam(5, 0);
          LedsAnim->setParam(7, 255);
          LedsAnim->setParam(8, 165);
          LedsAnim->setParam(9, 0);
        }
        LedsAnim->setParam(11, engine->power->SOC);

     }
  }


   if (engine->stm32->clicked()) {

     LedsAnim->setParam(11, 20);

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
