#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const int trig=9, echo=10;
const int motor=7;
int led[]={2,3,4};

long duration;
float distance;
int level;

void setup()
{
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(motor,OUTPUT);

  for(int i=0;i<3;i++)
    pinMode(led[i],OUTPUT);
}

void loop()
{
  // Measure Distance
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  duration = pulseIn(echo,HIGH);

  distance = duration * 0.0343 / 2.0;

  // Water level calculation
  level = 100 - (distance * 5);

  if(level>100) level=100;
  if(level<0) level=0;

  // LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Level:");
  lcd.print(level);
  lcd.print("%");

  // Turn OFF all LEDs
  for(int i=0;i<3;i++)
    digitalWrite(led[i],LOW);

  if(level<35)
  {
    digitalWrite(led[0],HIGH);
    digitalWrite(motor,HIGH);

    lcd.setCursor(0,1);
    lcd.print("LOW Pump ON ");
  }
  else if(level<70)
  {
    digitalWrite(led[1],HIGH);
    digitalWrite(motor,LOW);

    lcd.setCursor(0,1);
    lcd.print("MED Pump OFF");
  }
  else
  {
    digitalWrite(led[2],HIGH);
    digitalWrite(motor,LOW);

    lcd.setCursor(0,1);
    lcd.print("FULL PumpOFF");
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm  ");

  Serial.print("Level: ");
  Serial.print(level);
  Serial.println("%");

  delay(500);
}
