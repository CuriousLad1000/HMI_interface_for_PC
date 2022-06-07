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

int Res_Temp;
//========================= LCD =============================================
int LCD_TX = A3;
int LCD_RX = A4;
SoftwareSerial nextion(LCD_TX, LCD_RX);
//========================================================================================================


Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

void setup()
{
  Serial.begin(9600);
  myNextion.init();
  sensors1.begin();  // for ds18b20
  sensors2.begin();  // for ds18b20
  sensors3.begin();  // for ds18b20
  sensors4.begin();  // for ds18b20
  sensors5.begin();  // for ds18b20

}

void loop()
{
  //Res_Temp =  TempSys_TX();

  Res_Temp=TempSys_TX(sensors1);
}



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
}
