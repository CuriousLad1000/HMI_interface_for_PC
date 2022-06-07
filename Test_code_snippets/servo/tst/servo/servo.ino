#include <Servo.h>

Servo myservo;
int sys_stat_pin = 2; // input to detect PC status
int sys_stat2;
int flag_on = 2;

void setup()
{
  Serial.begin(9600);
  pinMode(sys_stat_pin, INPUT);
}

void loop()
{
  sys_status();
  delay(1000);
  if (sys_stat2 == 1)
  {
    myservo.attach(9);
    myservo.write(30);

    //  myservo.detach();
   // flag_on = 1;
    Serial.println(flag_on);
    if (flag_on == 1)
    {
      myservo.detach();
    }
    else if (flag_on == 2)
    {
      myservo.attach(9);
      myservo.write(30);
      flag_on = 1;
    }
  }
  else if (sys_stat2 == 0)
  {
    myservo.attach(9);
    myservo.write(90);
    delay(100);
    flag_on = 0;
    Serial.println(flag_on);
    if (flag_on == 0)
    {
      myservo.detach();
    }
  }
}


void sys_status()
{
  if (digitalRead(sys_stat_pin) == HIGH)
  {
    Serial.println("System is Online");
    sys_stat2 = 1;
  }
  else if (digitalRead(sys_stat_pin) == LOW)
  {
    Serial.println("System is Offline");
    sys_stat2 = 0;
  }
}
