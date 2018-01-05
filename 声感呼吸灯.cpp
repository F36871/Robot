#include <Adafruit_NeoPixel.h> /*引用”Adafruit_NeoPixel.h”文件。引用的意思有点象“复制-粘贴”。include文件提供了一种很方便的方式共享了很多程序共有的信息。*/
#define PIN 6                         /*定义了控制LED的引脚，6表示Microduino的D6引脚，可通过Hub转接出来，用户可以更改 */
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
 //该函数第一个参数控制串联灯的个数，第二个是控制用哪个pin脚输出，第三个显示颜色和变化闪烁频率
 
#define Pin_X A0
#define Pin_Y A1//麦克风接AO引脚
#define MIC_value1 50
#define MIC_value2 300
#define MIC_value3 500
#define MIC_value4 800
//D6，D8控制1A，1B的电机
#define OUT1A 6
#define OUT1B 8
//D5，D7控制2A，2B的电机
#define OUT2A 5
#define OUT2B 7

int value, data,sensorValue;

void setup()                                //创建无返回值函数
 {
  Serial.begin(9600);               //初始化串口通信，并将波特率设置为115200
  strip.begin();                             //准备对灯珠进行数据发送
  strip.show();                              //初始化所有的灯珠为关的状态
   pinMode(Pin_X, INPUT);
  pinMode(Pin_Y, INPUT);
  pinMode(OUT1A, OUTPUT);
  pinMode(OUT1B, OUTPUT);
  pinMode(OUT2A, OUTPUT);
  pinMode(OUT2B, OUTPUT);
  delay(500);
}
void motor_sta(int num, int fadeValue)
{
  if (num == 0)
  {
    analogWrite(OUT1A, fadeValue);
    digitalWrite(OUT1B, LOW);
    analogWrite(OUT2A, fadeValue);
    digitalWrite(OUT2B, LOW);
  }
  else if (num == 1)
  {
    digitalWrite(OUT1A, LOW);
    analogWrite(OUT1B, fadeValue);
    digitalWrite(OUT2A, LOW);
    analogWrite(OUT2B, fadeValue);
  }
}


int uiStep()        //切换转向
{
   if (analogRead(Pin_Y) > 50)  //Y-up
  {
    if (analogRead(Pin_Y) <300)
      return 1;
  }
    if (analogRead(Pin_Y) >300)  //Y-up
  {
    if (analogRead(Pin_Y) <500)
      return 2;
  }
    if (analogRead(Pin_Y) >500)  //Y-up
  {
    if (analogRead(Pin_Y)<800)
      return 3;
  }
  {
    if(analogRead(Pin_Y)<=50)
    return 4;
  }
  return 0;
}
void loop()                                  //无返回值loop函数
 {
  sensorValue = analogRead(Pin_X);             //声音检测
  Serial.println(sensorValue);                                //彩色led灯根据音频调节颜色和亮度
  if (sensorValue < MIC_value2)                          //若音频小于100
colorWipe(strip.Color(0, map(sensorValue, 50, 300, 0, 255), 0));
/*“map(val,x,y,m,n)”函数为映射函数，可将某个区间的值（x-y）变幻成（m-n），val则是你需要用来映射的数据，这里是将50到300的光对应用0到225的绿光标示*/
  else if (sensorValue >= MIC_value2 && sensorValue < MIC_value3)
  //若声音频率大于300小于500
     colorWipe(strip.Color(0, 0, map(sensorValue, 300, 500, 0, 255)));
    //将300到500的声音分别用0到255的蓝光表示
 else if (MIC_value3 >= 500)
colorWipe(strip.Color(map(sensorValue, 500, 800, 0, 255), 0, 0));
//将500到800的光用0到255的红光表示
if (uiStep() == 1)
  {
    value += 10;
    if (value > 255)
      value = 255;
  }
  else if (uiStep() == 2)
  {
    value -= 10;
    if (value < 0)
      value = 0;
  }
  else if (uiStep() == 3)
  data = 0;
  else if (uiStep() == 4)
    data=1;
  motor_sta(data, value);

  /*
  Serial.print(data);
  Serial.print("   ");
  Serial.println(value);
  */


  Serial.print(analogRead(Pin_Y));
  Serial.print("    ");
   Serial.print(analogReacd(Pin_X));
} 
void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++)  //i从0自增到LED灯个数减1
 {
    strip.setPixelColor(i, c); //将第i个灯点亮
    strip.show(); //led灯显示
  }
}
