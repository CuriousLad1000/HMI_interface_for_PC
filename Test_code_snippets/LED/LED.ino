#include <SoftwareSerial.h>
#include <Nextion.h>
#include "FastLED.h"
//===============================  PIN MAPPING ======================================================
//================= LED ========================
#define LED_CH1  A0
#define LED_CH2  A1
#define LED_CH3  A2
#define NUM_LEDS 8     // # of LEDS in the strip
CRGB leds_CH1[NUM_LEDS];
CRGB leds_CH2[NUM_LEDS];
CRGB leds_CH3[NUM_LEDS];
CRGBPalette16 Single_Fade = Nextion1_gp;
CRGBPalette16 myPal = temperature_gp;
CRGBPalette16 RBW = Rainbow_gp;
//========================= LCD =============================================
int LCD_TX = A3;
int LCD_RX = A4;
SoftwareSerial nextion(LCD_TX, LCD_RX);

Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

//========================== LED Var ==============================================
byte S_cntr;
byte S_sli1;
int S_del1;
byte chnl1;
byte chnl2;
byte chnl3;
byte F_cntr;
byte F_sli1;
int F_del1;
byte R_sli1;
int R_del1;
byte R_sli2;
int R_del2;
byte Tclr;
int clr_Temp;

void setup()
{
  Serial.begin(9600);
  myNextion.init();
  FastLED.addLeds<NEOPIXEL, LED_CH1>(leds_CH1, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_CH2>(leds_CH2, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_CH3>(leds_CH3, NUM_LEDS);
}

void loop()
{
  S_cntr = myNextion.getComponentValue("SINGLE.cntr_0"); // 1 to 16 values
  S_sli1 = myNextion.getComponentValue("SINGLE.Ssli1_0"); // 0 to 100 values
  S_del1 = myNextion.getComponentValue("SINGLE.Sdel1_0"); // 10 to 1000 values
  chnl1 = myNextion.getComponentValue("SINGLE.chnl1"); // 1 to 6 values
  chnl2 = myNextion.getComponentValue("SINGLE.chnl2"); // 1 to 6 values
  chnl3 = myNextion.getComponentValue("SINGLE.chnl3"); // 1 to 6 values
  F_cntr = myNextion.getComponentValue("FADE.cntr_0"); // 1 to 16 values
  F_sli1 = myNextion.getComponentValue("FADE.Fsli1_0"); // 0 to 100 values
  F_del1 = myNextion.getComponentValue("FADE.Fdel1_0"); // 10 to 1000 values
  R_sli1 = myNextion.getComponentValue("RAINBOW.Rsli1_0"); // 0 to 100 values
  R_del1 = myNextion.getComponentValue("RAINBOW.Rdel1_0"); // 10 to 1000 values
  R_sli2 = myNextion.getComponentValue("RAINBOW2.Rsli2_0"); // 0 to 100 values
  R_del2 = myNextion.getComponentValue("RAINBOW2.Rdel2_0"); // 10 to 1000 values
  Tclr = myNextion.getComponentValue("AUTOCLR.Tclr_0"); // 0 or 1 values
  S_cntr = constrain(S_cntr, 1, 16);
  S_sli1 = constrain(S_sli1, 0, 100);
  S_del1 = constrain(S_del1, 10, 1000);
  chnl1 = constrain(chnl1, 1, 6);
  chnl2 = constrain(chnl2, 1, 6);
  chnl3 = constrain(chnl3, 1, 6);
  F_cntr = constrain(F_cntr, 1, 16);
  F_sli1 = constrain(F_sli1, 0, 100);
  F_del1 = constrain(F_del1, 10, 1000);
  R_sli1 = constrain(R_sli1, 0, 100);
  R_del1 = constrain(R_del1, 10, 1000);
  R_sli2 = constrain(R_sli2, 0, 100);
  R_del2 = constrain(R_del2, 10, 1000);
  Tclr = constrain(Tclr, 0, 1);
  //=======================Add it after clr_Temp ==========================================================
  led();

}
//==================================================================================================================================================
//================================================================== FUNCTIONS =====================================================================
//==================================================================================================================================================

//================================================================== FOR Color Mapping ====================================================
void led()
{
  //=======================FOR CH1- modes =======================
  if (chnl1 == 5 || chnl2 == 5 || chnl3 == 5)
  {
    Temp_clr(clr_Temp);
  }
  else
  {
    switch (chnl1)
    {
      case 1: Single(S_cntr, S_sli1, 1, S_del1);
        break;
      case 2: Fade(F_cntr, F_sli1, 1, F_del1);
        break;
      case 3: Rainbow(R_sli1, 1, R_del1);
        break;
      case 4: Rainbow2(R_sli2, 1, R_del2);
        break;
      case 6: offline(1);
        break;
    }
    switch (chnl2)
    {
      case 1: Single(S_cntr, S_sli1, 2, S_del1);
        break;
      case 2: Fade(F_cntr, F_sli1, 2, F_del1);
        break;
      case 3: Rainbow(R_sli1, 2, R_del1);
        break;
      case 4: Rainbow2(R_sli2, 2, R_del2);
        break;
      case 6: offline(2);
        break;
    }
    switch (chnl3)
    {
      case 1: Single(S_cntr, S_sli1, 3, S_del1);
        break;
      case 2: Fade(F_cntr, F_sli1, 3, F_del1);
        break;
      case 3: Rainbow(R_sli1, 3, R_del1);
        break;
      case 4: Rainbow2(R_sli2, 3, R_del2);
        break;
      case 6: offline(3);
        break;
    }
  }
}

//===================================== FUNCTIONS =============================================================================
//================================================================== FOR SINGLE ====================================================
void Single(byte cntr, byte BRF, byte ch, int wait) // (S_cntr; S_sli1; Channel number; S_del1)
{
  BRF = BRF * 2.55;
  byte heatindex;
  heatindex = (16 * (cntr - 1));

  switch (ch)
  {
    case 1:
      {
        fill_palette( leds_CH1, NUM_LEDS, heatindex, 0, Single_Fade, BRF, LINEARBLEND);
        FastLED.show();
      }
      break;
    case 2:
      {
        fill_palette( leds_CH2, NUM_LEDS, heatindex, 0, Single_Fade, BRF, LINEARBLEND);
        FastLED.show();
      }
      break;
    case 3:
      {
        fill_palette( leds_CH3, NUM_LEDS, heatindex, 0, Single_Fade, BRF, LINEARBLEND);
        FastLED.show();
      }
      break;
  }
}
//======================================FADE============================================================================
void Fade(byte cntr, byte BRF, byte ch, int wait)
{
  byte heatindex;
  heatindex = (16 * (cntr - 1));
  byte light = BRF * 2.55;
  switch (ch)
  {
    case 1:
      {
        for (int i = 0; i < light; i++)
        {
          fill_palette( leds_CH1, NUM_LEDS, heatindex, 0, Single_Fade, i, LINEARBLEND);
          FastLED.show();
        }
        for (int i = light; i > 0; i--)
        {
          fill_palette( leds_CH1, NUM_LEDS, heatindex, 0, Single_Fade, i, LINEARBLEND);
          FastLED.show();
        }
        delay(wait);
      }
      break;
    case 2:
      {
        for (int i = 0; i < light; i++)
        {
          fill_palette( leds_CH2, NUM_LEDS, heatindex, 0, Single_Fade, i, LINEARBLEND);
          FastLED.show();
        }
        for (int i = light; i > 0; i--)
        {
          fill_palette( leds_CH2, NUM_LEDS, heatindex, 0, Single_Fade, i, LINEARBLEND);
          FastLED.show();
        }
        delay(wait);
      }
      break;
    case 3:
      {
        for (int i = 0; i < light; i++)
        {
          fill_palette( leds_CH3, NUM_LEDS, heatindex, 0, Single_Fade, i, LINEARBLEND);
          FastLED.show();
        }
        for (int i = light; i > 0; i--)
        {
          fill_palette( leds_CH3, NUM_LEDS, heatindex, 0, Single_Fade, i, LINEARBLEND);
          FastLED.show();
        }
        delay(wait);
      }
      break;
  }
}
//======================================================= FOR RAINBOW ===============================================================
void Rainbow(byte BRF, byte ch, int wait)
{
  static byte HI2;
  HI2 = HI2 + 1;
  BRF = BRF * 2.55;
  //Serial.println(HI2);
  switch (ch)
  {
    case 1:
      {
        fill_palette( leds_CH1, NUM_LEDS, HI2, 0, RBW, BRF, LINEARBLEND);
      }
      break;
    case 2:
      {
        fill_palette( leds_CH1, NUM_LEDS, HI2, 0, RBW, BRF, LINEARBLEND);
      }
      break;
    case 3:
      {
        fill_palette( leds_CH1, NUM_LEDS, HI2, 0, RBW, BRF, LINEARBLEND);
      }
      break;
  }
  FastLED.show();
  delay(wait);
}

//============================================================ FOR RAINBOW2 =============================================================
void Rainbow2(byte BRF, byte ch, int wait)
{
  static byte HI;
  HI = HI + 2;
  BRF = BRF * 2.55;
  //Serial.println(heatindex);
  switch (ch)
  {
    case 1:
      {
        fill_palette( leds_CH1, NUM_LEDS, HI, 20, RBW, BRF, LINEARBLEND);
      }
      break;
    case 2:
      {
        fill_palette( leds_CH2, NUM_LEDS, HI, 20, RBW, BRF, LINEARBLEND);
      }
      break;
    case 3:
      {
        fill_palette( leds_CH3, NUM_LEDS, HI, 20, RBW, BRF, LINEARBLEND);
      }
      break;
  }
  FastLED.show();
  delay(wait);
}


//============================================================ FOR AUTO COLOR =============================================================

void Temp_clr(int col_Temp)
{
  uint8_t heatindex;
  heatindex = map(col_Temp, 0, 70, 0, 255);
  for (int j = 0; j < NUM_LEDS; j++)
  {
    leds_CH1[j] = ColorFromPalette( myPal, heatindex); // normal palette access
    leds_CH2[j] = ColorFromPalette( myPal, heatindex); // normal palette access
    leds_CH3[j] = ColorFromPalette( myPal, heatindex); // normal palette access
  }
  FastLED.show();
}
//============================================================ FOR OFF =============================================================
void offline(byte ch)
{
  switch (ch)
  {
    case 1:
      {
        fill_solid( leds_CH1, NUM_LEDS, CRGB(0, 0, 0));
      }
      break;
    case 2:
      {
        fill_solid( leds_CH2, NUM_LEDS, CRGB(0, 0, 0));
      }
      break;
    case 3:
      {
        fill_solid( leds_CH3, NUM_LEDS, CRGB(0, 0, 0));
      }
      break;
  }
  FastLED.show();
}

//=====================================================================================================================================




