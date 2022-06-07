#include <SoftwareSerial.h>
#include <Nextion.h>
#include <Servo.h>

//========================= LCD =============================================
int LCD_TX = A3;
int LCD_RX = A4;
SoftwareSerial nextion(LCD_TX, LCD_RX);

Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

//======================================== SERVO ==============================================
Servo myservo;
int sys_stat_pin = 2; // input to detect PC status
int sys_stat2;
int flag_on = 0;
int flag_off = 0;
int pos;
int pos_auto;
int Res_Temp;
int flap_boot;
int flap_heat;


void setup()
{
  myNextion.init();
  myservo.attach(9);
  Serial.begin(9600);
  pinMode(sys_stat_pin, INPUT_PULLUP);
}

void loop()
{
  flap_boot = myNextion.getComponentValue("SET1.flapBoot"); // 0 or 1 value
  flap_heat = myNextion.getComponentValue("SET2.flapHeat"); // 0 or 1 value

  sys_status();
  Serial.print("sys stat: ");
  Serial.println(sys_stat2);
  Serial.print("flag_on: ");
  Serial.println(flag_on);
  Serial.print("flag_off: ");
  Serial.println(flag_off);
  if (flap_boot == 1)
  {
    srvo();
  }
  if(flap_heat==1)
  {
    srvo_auto();
  }
}

void srvo()
{
  if (flag_on == 0)         //  One time flag
  {
    if (sys_stat2 == 1)  // sys. On flag
    {
      myservo.attach(9);
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
      myservo.attach(9);
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
  pos_auto = map(Res_Temp, 0, 70, 30, 90);
  myservo.attach(9);
  myservo.write(pos_auto);
}

void sys_status()
{
  if (digitalRead(sys_stat_pin) == !HIGH)
  {
    //  Serial.println("System is Online");
    sys_stat2 = 1;
  }
  else if (digitalRead(sys_stat_pin) == !LOW)
  {
    //   Serial.println("System is Offline");
    sys_stat2 = 0;
  }
}
