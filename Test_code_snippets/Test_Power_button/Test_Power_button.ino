#include <SoftwareSerial.h>
#include <Nextion.h>

int LCD_TX = A3;
int LCD_RX = A4;
int sys_pow_pin = A5;  // CPU ON/OFF switch pin
int sys_led_pin = 1;  // CPU LED
int sys_stat_pin = 2; // input to detect PC status
int sys_stat;
int sys_stat2;
int sys_led;
int sys_switch_prev = 0;
int sys_switch;

SoftwareSerial nextion(LCD_TX, LCD_RX);
Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps


void setup()
{
  pinMode(sys_pow_pin, OUTPUT);
  digitalWrite(sys_pow_pin, HIGH);
  pinMode(sys_led_pin, OUTPUT);
  pinMode(sys_stat_pin, INPUT);
  //  Serial.begin(9600);
  myNextion.init();
}

void loop()
{
  //======================== Detect Button Press ===========================
  sys_switch = myNextion.getComponentValue("HOME.n0"); // 0 or 1 value
  sys_led = myNextion.getComponentValue("SET1.sys_led"); // 0 or 1 value

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
  Serial.println();
  //=================================== System LED status =======================
  system_LED();
}

//=======================================================================================================================================


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

