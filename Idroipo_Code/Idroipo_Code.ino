#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <LCD5110_Graph.h>
#include <EEPROM.h>

//WATER SENSOR
#define pinWater 5

//LIGHT RESISTOR
#define pinLight A0
#define ValueLight 512

//DF PLAYER
SoftwareSerial mySoftwareSerial(6,7); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

//DISPLAY
#define lightLed 13
extern unsigned char BigNumbers[];
extern uint8_t happyFace[];
extern uint8_t lightFace[];
extern uint8_t waterFace[];
//pin names in order: sclk, dn mosi, dc, rst, sce
LCD5110 lcd(12, 11, 10, 8, 9);

//TIME
float spendTime;
float lastTime;
float hours;

//BUTTONS
#define B_resetTime A5
#define B_readTime A4
#define B_sayInfo A3
#define B_saytutorial A2 
int delayAudio; 
int info;


void setup() {
  pinMode(pinWater, INPUT);
  pinMode(lightLed, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("start");
  
  //df player
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);
  delayAudio=millis();
  info=2;
  //time
  readTime();
  lastTime=millis();
  //lcd
  /*lcd.InitLCD();  //initialize LCD screen
  lcd.setFont(BigNumbers);  //set font for LCD*/

  Serial.println("start");
}

void loop() {
  /*boolean happy=true;
  if(checkWater)
  {
    showDisplay("water");
    happy=false;
  }
  if(checkLight && happy)
  {
    showDisplay("light");
    happy=false;
  }
  if(happy)
  {
    showDisplay("happy");
  }*/
  manageButtons();
  //manageTime();
  delay(10);
}

void showDisplay(String face)
{
  lcd.clrScr();
  if(face=="light")
    lcd.drawBitmap(0, 0, lightFace, 84, 48);
  if(face=="water")
    lcd.drawBitmap(0, 0, waterFace, 84, 48);
  if(face=="happy")
    lcd.drawBitmap(0, 0, happyFace, 84, 48);
  lcd.update(); 
}
void lightDisplay(boolean light)
{
  if(light)
    digitalWrite(lightLed, LOW);//turno on
  else
    digitalWrite(lightLed, HIGH);//turn off
}

boolean checkLight(){
  if(analogRead(pinLight)>=ValueLight)
    return true;
  return false;
}

boolean checkWater(){
  if(digitalRead(pinWater)==1)
    return true;
  return false;
}

void manageButtons(){
  if(analogRead(B_resetTime)==1023 && millis()-delayAudio>=1000)
  {
    resetTime();
  }
  if(analogRead(B_readTime)==1023 && millis()-delayAudio>=1000)
  {
    playAudio("days "+readDays());
    delayAudio=millis();
  }
  if(analogRead(B_sayInfo)==1023 && millis()-delayAudio>=1000)
  {
    playAudio("info");
    delayAudio=millis();
  }
  if(analogRead(B_saytutorial)==1023 && millis()-delayAudio>=1000)
  {
    playAudio("tutorial");
    delayAudio=millis();
  }
}

void manageTime(){
  if(lastTime<millis())
    spendTime=spendTime+(lastTime-millis());
  else
    spendTime=spendTime+millis();
  lastTime=millis();
  if(spendTime/3600000>hours)//Check when pass 1 hour
  {//after 1 hour I save the time
    writeTime();
    hours=hours+1;
  }
}
void resetTime(){
  for(int i=0;i<1024;i++)
    EEPROM.update(i,0);//Addr, Value (update=write only if the value changes)
  hours=0;
  spendTime=0;
  lastTime=millis();
}
void readTime(){
  spendTime=0;
  int a=0;
  int number=-1;
  while(a<=32)//2^32 = 4 miliardi di millisecondi = + di 40 giorni
  {
    number=EEPROM.read(a);
    spendTime=spendTime+number*pow(10,a);
    a++;
  }
  hours=(int)(spendTime/3600000);
}
void writeTime(){
  int tmp=spendTime;
  int a=0;
  while(tmp>0)
  {
    int rest=tmp%10;
    EEPROM.write(a,rest);
    a++;
    tmp=tmp/10;
  }
}
int readDays()
{
  int days=(int)(hours/24);
  return days;
}

void playAudio(String audio){
  myDFPlayer.play(1);  //Play the first mp3
  //Tutorial

  //Info
  if(audio=="info")
  {
    myDFPlayer.play(info);
    info++;
    if(info==12)
      info=2;
  }
 
  //Days
  if(audio=="day 0")
    myDFPlayer.play(1);
}
