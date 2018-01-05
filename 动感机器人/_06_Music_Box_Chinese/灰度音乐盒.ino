

#include <Microduino_Key.h>
#include "colorLed.h"
#include "music.h"
#include "userDef.h"

AnalogKey keyLine(PIN_LINEA);

uint32_t cardTimer = 0;
uint8_t cardNum = 0;
uint8_t playNum = 0;

void getCard()
{
  if (keyLine.readEvent(0, DOCK_MAX) == SHORT_PRESS)
  {
    setAllColor(COLOR_BLUE);
    cardNum++;
    cardTimer = millis();
  }
}

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  keyLine.begin(INPUT);
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
#if DEBUG
  Serial.print("LINE Val:");
  Serial.println(analogRead(PIN_LINEA));      //灰度传感器检测到的亮度。
#endif
  getCard();
  if (millis() - cardTimer > 1000 && cardTimer > 0) //1秒钟内没有检测到新的条纹，即认为刷卡结束。
  {
    noTone(PIN_BUZZER);
    setAllColor(COLOR_NONE);
    playNum = constrain(cardNum - 2, 0, SONG_SUM - 1); //根据扫描到的黑色条纹数播放相应的音乐。
    cardNum = 0;
    playIndex = 0;
    cardTimer = 0;
    allLedBlinkNum(playNum, COLOR_WARM, 500);
  }
  if (!playSound(playNum))//播放音乐。
  {
    noTone(PIN_BUZZER);
    ledBreath(COLOR_GREEN, 15);
  }
  delay(15);
}
