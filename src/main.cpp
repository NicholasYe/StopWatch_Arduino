#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <SPI.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9

#define OLED_RESET 4
Adafruit_SSD1306 oled(128, 64, &Wire, OLED_RESET);

LiquidCrystal lcd(3, 5, 6, 7, 8, 9);//定义LCD的引脚

int second, minute, hour = 0;
int state = 0;

//刷新LCD
void refresh()
{
  lcd.clear();
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);
  Serial.print("Lcd refreshed");
  Serial.print("   ");
  Serial.println(state);
}

//外部中断，外部电平改变后，触发中断
void change_state()
{
  Serial.println("state changed");
  if (state == 0 || state == 1 || state == 2)
  {
    state++;
  }
  else if (state == 3)
  {
    state = 0;
    refresh();
  }
}

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  lcd.begin(16, 2);
  Serial.println("RC522初始化完成...");
  attachInterrupt(0, change_state, RISING);
}

void loop()
{
  //初始暂停状态
  if (state == 0)
  {
    hour = 0;
    minute = 0;
    second = 0;
    refresh();
  }
  //开始计时状态
  else if (state == 2)
  {
    do
    {
      delay(1000);
      if (second < 60)
      {
        second++;
      }
      else
      {
        second = 0;
        minute++;
      }
      if (minute == 60)
      {
        minute = 0;
        hour++;
      }
      refresh();
    } while (state == 2);
  }
}