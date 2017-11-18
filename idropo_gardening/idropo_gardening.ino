/*
Interaction Design Studio
Politecnico di Milano - Scuola del Design
A.A. 2016/2017
--------------------------
IDROPO HYDROPONIC SYSTEM
--------------------------
Gruppo SHOUT:
Federica Carrozzo
Ruben Faccini
Giada Guatto
Beatrice Redaelli
*/

#include <SoftwareSerial.h>
#include <LCD5110_Graph.h>

//Bluetooth
SoftwareSerial BTserial(10, 11); // RX | TX

//LCD pins, font and icons

//pin names in order: sclk, dn mosi, dc, rst, sce
LCD5110 lcd(7, 6, 5, 4, 3);

char c = ' ';

extern unsigned char BigNumbers[];
extern uint8_t happy[];
extern uint8_t light[];
extern uint8_t water[];


//LDR  sensor int
int ldrPin = A0;
int ldrValue = 0; // variable to store the value coming from the sensor

//distance sensor
int trigPin = 9;
int echoPin = 8;
long duration;
int distance;


void setup() {
  BTserial.begin(38400);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ldrPin, INPUT);
  lcd.InitLCD();  //initialize LCD screen
  lcd.setFont(BigNumbers);  //set font for LCD
  Serial.begin(9600);
  Serial.println("hc-05 is ready");

}

void loop() {

  //Read and convert distance to Centimeters
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance ");
  Serial.println(distance);

  int ldrValue = analogRead(ldrPin);
  Serial.print("LDR ");
  Serial.println(ldrValue);
  

  String str = String(distance) + " - " + String(ldrValue);
  BTserial.println(str);


    if (ldrValue < 800) { // low light
      lcd.clrScr();
      lcd.drawBitmap(0, 0, light, 84, 48);
      lcd.update(); 
  }

      else if (distance > 5) { // low water
      lcd.clrScr();
      lcd.drawBitmap(0, 0, water, 84, 48);
      lcd.update();
  }

  else {  // distance and light ok
      lcd.clrScr();
      lcd.drawBitmap(0, 0, happy, 84, 48);
      lcd.update();

  } 
 } 

