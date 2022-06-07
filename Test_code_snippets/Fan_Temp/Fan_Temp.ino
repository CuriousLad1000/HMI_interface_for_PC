#include <SoftwareSerial.h>
#include <Nextion.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//===============================  PIN MAPPING ======================================================
//======================== Temperature Sensors=========================================

//===================== DS18b20(CH1_TMP) ========================
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

//================= FAN ========================
int FAN_CH1 = 3;
int FAN_CH2 = 5;
int FAN_CH3 = 6;
int FAN_CH4 = 11;
String loc1 = "FAN.fanSli1";
String loc2 = "FAN.fanSli2";
String loc3 = "FAN2.fanSli3";
String loc4 = "FAN2.fanSli4";
//========================= LCD =============================================
int LCD_TX = A3;
int LCD_RX = A4;
SoftwareSerial nextion(LCD_TX, LCD_RX);

Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

//========================== FAN ==============================================
int ch1_SLI;
int ch2_SLI;
int ch3_SLI;
int ch4_SLI;
int auto_tmp;
int Res_Temp;   //for auto fan control


void setup()
{
  Serial.begin(9600);
  myNextion.init();
  sensors1.begin();  // for ds18b20
  sensors2.begin();  // for ds18b20
  sensors3.begin();  // for ds18b20
  sensors4.begin();  // for ds18b20
  sensors5.begin();  // for ds18b20

  pinMode(FAN_CH1, OUTPUT);
  pinMode(FAN_CH2, OUTPUT);
  pinMode(FAN_CH3, OUTPUT);
  pinMode(FAN_CH4, OUTPUT);
}

void loop()
{
  ch1_SLI = myNextion.getComponentValue("FAN.fanSli1");  // 0 to 100 values
  ch2_SLI = myNextion.getComponentValue("FAN.fanSli2"); // 0 to 100 values
  ch3_SLI = myNextion.getComponentValue("FAN2.fanSli3"); // 0 to 100 values
  ch4_SLI = myNextion.getComponentValue("FAN2.fanSli4"); // 0 to 100 values
  auto_tmp = myNextion.getComponentValue("FAN.auto"); // 0 or 1 value

  //auto_tmp = 1;

  //===============================  Temperature CODE  ===================================================================

  Res_Temp = TempSys_TX(sensors1);
  //  Res_Temp = 50;
  Res_Temp = constrain(Res_Temp, 0, 70); // fan at full speed if temp. hits 70;
  // Serial.println(Res_Temp);
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
  // delay(1000);
}
//==================================================================================================================================================
//================================================================== FUNCTIONS =====================================================================
//==================================================================================================================================================

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
    Serial.println(value);
    analogWrite(fan_ch, value);

    value = map(value, 0, 255, 0, 100);
    myNextion.setComponentValue(LOC, value);
  }

}

//===========================================================  TEMPERATURE Func ====================================================
int TempSys_TX(DallasTemperature sensors)
{
  int Temp;
  sensors.requestTemperatures(); // Send the command to get temperatures
  float Res1 = sensors.getTempCByIndex(0);
  int Res1a = (int)Res1;
  String a =  (String(Res1) + "\xb0" + "C");  //xb0 is for degree symbol
  //  Serial.println(a);
  myNextion.setComponentText("TMP1.tmp0", a);
  int scaled_value1 = map(Res1a, 0, 100, 0, 100); // always map value from 0 to 100
  myNextion.setComponentValue("TMP1.prg0", scaled_value1);
  // Serial.println(a);
  Temp = Res1a;
  //Serial.println(Temp);
  return (Temp);
}

//=====================================================================================================================================

