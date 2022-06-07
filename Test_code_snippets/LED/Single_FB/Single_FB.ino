
#include <Adafruit_NeoPixel.h>
//===============================  PIN MAPPING ======================================================

#define BACK_LED    A0    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 8
//#define PIXEL_COUNT2 16
Adafruit_NeoPixel stripR = Adafruit_NeoPixel(PIXEL_COUNT, BACK_LED, NEO_GRB + NEO_KHZ800);

//=================FOR Single/Moving mode=========================

// Back====
uint32_t blueR = stripR.Color(0, 0, 255);
uint32_t cyanR = stripR.Color(0, 255, 255);
uint32_t darkBlueR = stripR.Color(0, 128, 255);
uint32_t greenR = stripR.Color(0, 255, 0);
uint32_t lightBlueR = stripR.Color(0, 191, 255);
uint32_t lightGreenR = stripR.Color(64, 255, 0);
uint32_t magentaR = stripR.Color(255, 0, 255);
uint32_t maroonR = stripR.Color(255, 0, 64);
uint32_t orangeR = stripR.Color(255, 128, 0);
uint32_t parrotGreenR = stripR.Color(191, 255, 0);
uint32_t pinkR = stripR.Color(255, 0, 191);
uint32_t purpleR = stripR.Color(128, 0, 255);
uint32_t redR = stripR.Color(255, 0, 0);
uint32_t violetR = stripR.Color(191, 0, 255);
uint32_t whiteR = stripR.Color(255, 255, 255);
uint32_t yellowR = stripR.Color(255, 255, 0);

//== led==

int delR;

int CR;

int BRR;



void setup()
{

  Serial.begin(9600);
  stripR.begin();
  stripR.show(); // Initialize all pixels to 'off'
}

void loop()
{
  delR = 50;

  CR = 1;

  BRR = 20;


  SingleR();

  //=====================================================================================================================================
  //Serial.println("End of loop");
}


//=========================================================== LED =================================================================

void SingleR()
{
  int BRR2 = BRR * 2.55;
  stripR.setBrightness(BRR2);

  uint32_t col;
  switch (CR) {
    case 1: col = blueR;
      break;
    case 2: col = cyanR;
      break;
    case 3: col = darkBlueR;
      break;
    case 4: col = greenR;
      break;
    case 5: col = lightBlueR;
      break;
    case 6: col = lightGreenR;
      break;
    case 7: col = magentaR;
      break;
    case 8: col = maroonR;
      break;
    case 9: col = orangeR;
      break;
    case 10: col = parrotGreenR;
      break;
    case 11: col = pinkR;
      break;
    case 12: col = purpleR;
      break;
    case 13: col = redR;
      break;
    case 14: col = violetR;
      break;
    case 15: col = whiteR;
      break;
    case 16: col = yellowR;
      break;
  }

  colorWipeR(col, delR);
}
//======================= For Single Mode==================================
void colorWipeR(uint32_t c, uint8_t wait)
{

  for (uint16_t i = 0; i < stripR.numPixels(); i++)
  {
    stripR.setPixelColor(i, c);
    stripR.show();
    delay(wait);
  }
}
