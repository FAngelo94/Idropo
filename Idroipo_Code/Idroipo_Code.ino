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
SoftwareSerial mySoftwareSerial(7,6); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

//DISPLAY
#define lightLed 13
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


void setup() {
  pinMode(pinWater, INPUT);
  pinMode(lightLed, OUTPUT);
  //df player
  mySoftwareSerial.begin(9600);
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  //time
  readTime();
  lastTime=millis();
}

void loop() {
  boolean happy=true;
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
  }
  manageButtons();
  manageTime();
  delay(50);
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
    digitalWrite(lightLed, HIGH);
  else
    digitalWrite(lightLed, LOW);
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
  if(analogRead(B_resetTime)>3)
  {
    resetTime();
  }
  if(analogRead(B_readTime)>3)
  {
    playAudio("days");
  }
  if(analogRead(B_sayInfo)>3)
  {
    playAudio("info");
  }
  if(analogRead(B_saytutorial)>3)
  {
    playAudio("tutorial");
  }
}

void manageTime(){
  if(lastTime<millis())
    spendTime=spendTime+(lastTime<millis());
  else
    spendTime=spendTime+millis();
    lastTime=millis();
    if(spendTime/3600000>hours)//Check when pass 1 hour
    {
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
  int spendTime=0;
  int a=0;
  int number=-1;
  while(number!=0)
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

void playAudio(String audio){
 myDFPlayer.play(1);  //Play the first mp3
}
