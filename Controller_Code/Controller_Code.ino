#include <SoftwareSerial.h>
#include <Nextion.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FastLED.h"
#include <Servo.h>
//===============================  PIN MAPPING ======================================================
//=================== POW BUTTON =============================
int sys_pow_pin = A5;  // CPU ON/OFF switch pin
int sys_led_pin = 1;  // CPU LED
int sys_stat_pin = 2; // input to detect PC status

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
//================= FAN ========================
int FAN_CH1 = 3;
int FAN_CH2 = 5;
int FAN_CH3 = 6;
int FAN_CH4 = 11;

//======================== Temperature Sensors=========================================
int CH1_TMP = 4;
OneWire oneWire1(CH1_TMP);
DallasTemperature sensors1(&oneWire1);
//===================== DS18b20(CH2_TMP) ========================
int CH2_TMP = 7;
OneWire oneWire2(CH2_TMP);
DallasTemperature sensors2(&oneWire2);
//===================== DS18b20(CH3_TMP) ========================
int CH3_TMP = 8;
OneWire oneWire3(CH3_TMP);
DallasTemperature sensors3(&oneWire3);
//===================== DS18b20(CH4_TMP) ========================
int CH4_TMP = 12;
OneWire oneWire4(CH4_TMP);
DallasTemperature sensors4(&oneWire4);
//===================== DS18b20(CH5_TMP) ========================
int CH5_TMP = 13;
OneWire oneWire5(CH5_TMP);
DallasTemperature sensors5(&oneWire5);
//===========================SERVO PIN===========================================
Servo myservo;
int sv1 = 9;
int sv2 = 10;

//========================= LCD =============================================
int LCD_TX = A3;
int LCD_RX = A4;
SoftwareSerial nextion(LCD_TX, LCD_RX);
Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps



//============================SYS POW var.============================================
int sys_stat;
int sys_stat2;
int sys_led;
int sys_switch_prev = 0;
int sys_switch;

//========================== FAN ==============================================
String loc1 = "FAN.fanSli1";
String loc2 = "FAN.fanSli2";
String loc3 = "FAN2.fanSli3";
String loc4 = "FAN2.fanSli4";
int ch1_SLI;
int ch2_SLI;
int ch3_SLI;
int ch4_SLI;
int auto_tmp;
int Res_Temp;   //for auto fan control
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
//============================SERVO var.=================================
int flag_on = 0;
int flag_off = 0;
int pos;
int pos_auto;
int flap_boot;
int flap_heat;
//=========================================================================

void setup()
{
  //Serial.begin(9600);

  sensors1.begin();  // for ds18b20
  sensors2.begin();  // for ds18b20
  sensors3.begin();  // for ds18b20
  sensors4.begin();  // for ds18b20
  sensors5.begin();  // for ds18b20

  FastLED.addLeds<NEOPIXEL, LED_CH1>(leds_CH1, NUM_LEDS); // for fastLED ch1
  FastLED.addLeds<NEOPIXEL, LED_CH2>(leds_CH2, NUM_LEDS); // for fastLED ch2
  FastLED.addLeds<NEOPIXEL, LED_CH3>(leds_CH3, NUM_LEDS); // for fastLED ch3

  pinMode(sys_pow_pin, OUTPUT);
  digitalWrite(sys_pow_pin, HIGH);
  pinMode(sys_led_pin, OUTPUT);
  pinMode(sys_stat_pin, INPUT);

  pinMode(FAN_CH1, OUTPUT);
  pinMode(FAN_CH2, OUTPUT);
  pinMode(FAN_CH3, OUTPUT);
  pinMode(FAN_CH4, OUTPUT);
  myNextion.init();

  myservo.attach(sv1);
  //myservo.attach(sv2);
}

void loop()
{
  ch1_SLI = myNextion.getComponentValue("FAN.fanSli1");  // 0 to 100 values
  ch2_SLI = myNextion.getComponentValue("FAN.fanSli2"); // 0 to 100 values
  ch3_SLI = myNextion.getComponentValue("FAN2.fanSli3"); // 0 to 100 values
  ch4_SLI = myNextion.getComponentValue("FAN2.fanSli4"); // 0 to 100 values
  auto_tmp = myNextion.getComponentValue("FAN.auto"); // 0 or 1 value

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

  sys_switch = myNextion.getComponentValue("HOME.n0"); // 0 or 1 value
  sys_led = myNextion.getComponentValue("SET1.sys_led"); // 0 or 1 value

  flap_boot = myNextion.getComponentValue("SET1.flapBoot"); // 0 or 1 value
  flap_heat = myNextion.getComponentValue("SET1.flapHeat"); // 0 or 1 value

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
  /*Serial.print("chnl1: ");
    Serial.println(chnl1);
    Serial.print("chnl2: ");
    Serial.println(chnl2);
    Serial.print("chnl3: ");
    Serial.println(chnl3);
    Serial.print("R_sli1: ");
    Serial.println(R_sli1);
    Serial.print("R_del1: ");
    Serial.println(R_del1);*/

  //===============================  Temperature CODE  ===================================================================

  Res_Temp = TempSys_TX(sensors1, 1);  // (Sensor number; channel number)
  //  Res_Temp = 50;
  Res_Temp = constrain(Res_Temp, 0, 70); // fan at full speed if temp. hits 70;
  clr_Temp = Res_Temp;
  // Serial.println(Res_Temp);
  int Res_Temp2 = TempSys_TX(sensors2, 2);
  Res_Temp2 = TempSys_TX(sensors3, 3);
  Res_Temp2 = TempSys_TX(sensors4, 4);
  Res_Temp2 = TempSys_TX(sensors5, 5);
  //=========================================================================================== Fan Auto =============
  if (auto_tmp == 1)
  {
    MAP_FAN_AUTO(FAN_CH1, loc1);
    MAP_FAN_AUTO(FAN_CH2, loc2);
    MAP_FAN_AUTO(FAN_CH3, loc3);
    MAP_FAN_AUTO(FAN_CH4, loc4);
  }
  else if (auto_tmp == 0)
  {
    MAP_FAN(FAN_CH1, ch1_SLI);
    MAP_FAN(FAN_CH2, ch2_SLI);
    MAP_FAN(FAN_CH3, ch3_SLI);
    MAP_FAN(FAN_CH4, ch4_SLI);
  }
  //=================================LED CODE=================================================================
  led();

  //=========================================SYSTEM POWER CODE==========================================================
  //=== Detect Button Press ==

  if (sys_switch == -1)
  {
  }
  else
  {
    //Serial.print("sys_switch: ");
    //Serial.println(sys_switch);
    if (sys_switch != sys_switch_prev)
    {
      if (sys_stat == 0)
      {
        //Serial.println("Firing UP the PC");
        sys_stat = 1;
        sys_pow(); // SEND SIGNAL TO BOOT PC
      }
      else if (sys_stat == 1)
      {
        //Serial.println("Shutting Down PC");
        sys_stat = 0;
        sys_pow();// SEND SIGNAL TO SHUTDOWN PC
      }
    }
    sys_switch_prev = sys_switch;
  }
  sys_status();

  //============================= This code is changing the front pic of button========================================
  if (sys_stat2 == 1)
  {
    myNextion.sendCommand("HOME.ON.picc=5");  // changes picc
  }

  else if (sys_stat2 == 0)
  {
    myNextion.sendCommand("HOME.ON.picc=0");
  }
  //Serial.println();
  //=================================== System LED status =======================
  system_LED();

  //=================================== SERVO FLAP ==============================================
  // Serial.print("sys stat: ");
  // Serial.println(sys_stat2);
  // Serial.print("flag_on: ");
  // Serial.println(flag_on);
  // Serial.print("flag_off: ");
  // Serial.println(flag_off);
  if (flap_boot == 1 && flap_heat == 0)
  {
    srvo();
  }
  if (flap_heat == 1 && sys_stat2 == 1 && flap_boot == 0)
  {
    srvo_auto();
  }
 // myservo.detach();
}





//==================================================================================================================================================
//================================================================== FUNCTIONS =====================================================================
//==================================================================================================================================================
//=========================================SYSTEM FUNC.==========================================================
void system_LED()
{
  if (sys_stat2 == 1 && sys_led == 1)
  {
    digitalWrite(sys_led_pin, HIGH);
  }
  else
  {
    digitalWrite(sys_led_pin, LOW);
  }
}

void sys_status()
{
  if (digitalRead(sys_stat_pin) == HIGH)
  {
    sys_stat2 = 1;
  }
  else
  {
    sys_stat2 = 0;
  }
}

void sys_pow()
{
  digitalWrite(sys_pow_pin, LOW);
  delay(500);
  digitalWrite(sys_pow_pin, HIGH);
}


//=========================================== FAN ================================================
void MAP_FAN(int fan_ch, int ch_sli)
{
  if (ch_sli == 0)
  {
    analogWrite(fan_ch, 0); //0 and 40 to 255
  }
  else if (ch_sli > 0 && ch_sli < 16)
  {
    ch_sli = 40;
    analogWrite(fan_ch, ch_sli); //0 and 40 to 255
    //Serial.println(ch_sli);
  }
  else
  {
    ch_sli = map(ch_sli, 0, 100, 0, 255);
    // Serial.println(ch_sli);
    analogWrite(fan_ch, ch_sli); //0 and 40 to 255
  }
}


void MAP_FAN_AUTO(int fan_ch, String LOC)
{
  int value;
  if (Res_Temp > 0 && Res_Temp < 12)
  {
    value = 40;
    analogWrite(fan_ch, value);

    value = map(value, 0, 255, 0, 100);
    myNextion.setComponentValue(LOC, value);
  }
  else
  {
    value = map(Res_Temp, 0, 70, 0, 255);
    //Serial.println(value);
    analogWrite(fan_ch, value);

    value = map(value, 0, 255, 0, 100);
    myNextion.setComponentValue(LOC, value);
  }

}

//===========================================================  TEMPERATURE Func ====================================================
int TempSys_TX(DallasTemperature sensors, int ch_no)
{
  int Temp;
  sensors.requestTemperatures(); // Send the command to get temperatures
  float Res1 = sensors.getTempCByIndex(0);
  int Res1a = (int)Res1;
  String a =  (String(Res1) + "\xb0" + "C");  //xb0 is for degree symbol
  //  Serial.println(a);
  int scaled_value1 = map(Res1a, 0, 100, 0, 100); // always map value from 0 to 100
  if (ch_no == 1)
  {
    myNextion.setComponentText("TMP1.tmp0", a);
    myNextion.setComponentValue("TMP1.prg0", scaled_value1);
  }
  else if (ch_no == 2)
  {
    myNextion.setComponentText("TMP1.tmp1", a);
    myNextion.setComponentValue("TMP1.prg1", scaled_value1);
  }
  else if (ch_no == 3)
  {
    myNextion.setComponentText("TMP1.tmp2", a);
    myNextion.setComponentValue("TMP1.prg2", scaled_value1);
  }
  else if (ch_no == 4)
  {
    myNextion.setComponentText("TMP2.tmp3", a);
    myNextion.setComponentValue("TMP2.prg3", scaled_value1);
  }
  else if (ch_no == 5)
  {
    myNextion.setComponentText("TMP2.tmp4", a);
    myNextion.setComponentValue("TMP2.prg4", scaled_value1);
  }
  // Serial.println(a);
  Temp = Res1a;
  //Serial.println(Temp);
  return (Temp);
}

//================================================================== FOR LEDs ====================================================
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
        fill_palette( leds_CH2, NUM_LEDS, HI2, 0, RBW, BRF, LINEARBLEND);
      }
      break;
    case 3:
      {
        fill_palette( leds_CH3, NUM_LEDS, HI2, 0, RBW, BRF, LINEARBLEND);
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
//============================================SERVO FUNCTIONS====================================================================
void srvo()
{
  if (flag_on == 0)         //  One time flag
  {
    if (sys_stat2 == 1)  // sys. On flag
    {
      myservo.attach(sv1);
      for (pos = 30; pos <= 90; pos += 1)
      {
        myservo.write(pos);
        delay(15);
      }

      // myservo.write(30);  //open
      flag_on = 1;
      delay(1000);
    }
  }

  else if (flag_on == 1)
  {
    myservo.detach();
  }

  if (flag_off == 0)
  {
    if (sys_stat2 == 0)
    {
      myservo.attach(sv1);
      for (pos = 90; pos >= 30; pos -= 1)
      {
        myservo.write(pos);
        delay(15);
      }
      //     myservo.write(90);  //close
      flag_off = 1;
      delay(1000);
    }
  }
  else if (flag_off == 1)
  {
    myservo.detach();
    if (sys_stat2 == 0 && flag_on == 1 && flag_off == 1)
    {
      flag_off = 0;
      flag_on = 0;
    }
  }
}

void srvo_auto()
{
  Res_Temp = constrain(Res_Temp, 0, 70);
  pos_auto = map(Res_Temp, 0, 70, 90, 30);
  pos_auto = constrain(pos_auto, 30, 90);
  if (pos_auto >= 60)
  {
    myservo.detach();
  }
  else
  {
    myservo.attach(sv1);
    myservo.write(pos_auto);
  }
  //Serial.println(pos_auto);

}
//=====================================================================================================================================




