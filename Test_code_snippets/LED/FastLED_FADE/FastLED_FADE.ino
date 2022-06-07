#include <SoftwareSerial.h>
#include <Nextion.h>
#include <Adafruit_NeoPixel.h>
#include "FastLED.h"
#define NUM_LEDS 32  // # of LEDS in the strip
CRGB leds_CH1[NUM_LEDS];
CRGB leds_CH2[NUM_LEDS];
CRGB leds_CH3[NUM_LEDS];
CRGBPalette16 Single_Fade = Nextion1_gp;
CRGBPalette16 myPal = temperature_gp;
CRGBPalette16 RBW = Rainbow_gp;
#define LED_CH1  A0
#define LED_CH2  A1
#define LED_CH3  A2

Adafruit_NeoPixel strip_CH1 = Adafruit_NeoPixel(NUM_LEDS, LED_CH1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_CH2 = Adafruit_NeoPixel(NUM_LEDS, LED_CH2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_CH3 = Adafruit_NeoPixel(NUM_LEDS, LED_CH3, NEO_GRB + NEO_KHZ800);
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
  //Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_CH1>(leds_CH1, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_CH2>(leds_CH2, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_CH3>(leds_CH3, NUM_LEDS);
  strip_CH1.begin();
  strip_CH1.show(); // Initialize all pixels to 'off'
  strip_CH2.begin();
  strip_CH2.show(); // Initialize all pixels to 'off'
  strip_CH3.begin();
  strip_CH3.show(); // Initialize all pixels to 'off'

}

void loop()
{
  S_cntr = 11; // 1 to 16 values
  S_sli1 = 20; // 0 to 100 values
  S_del1 = 10; // 10 to 1000 values

  chnl1 = 1; // 1 to 6 values
  chnl2 = 3; // 1 to 6 values
  chnl3 = 4; // 1 to 6 values

  F_cntr = 1; // 1 to 16 values
  F_sli1 = 20; // 0 to 100 values
  F_del1 = 1; // 10 to 1000 values

  R_sli1 = 20; // 0 to 100 values
  R_del1 = 1; // 10 to 1000 values

  R_sli2 = 20; // 0 to 100 values
  R_del2 = 1; // 10 to 1000 values
  Tclr = 0; // 0 or 1 values
  clr_Temp = 30; // temperature value  between 0 to 70


  Single(S_cntr, S_sli1, 1, S_del1);
  //Fade(F_cntr, F_sli1, 2, F_del1);

  //Rainbow2(R_sli1, 3, R_del1);
  // Rainbow(R_sli2, 1, R_del2);

  //Temp_clr(clr_Temp);
  //offline(1);
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
  Serial.println(HI2);
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




