/*
* Name: Solföljare 
* Author: Ludvig Thulin
* Date: 2025-04-13
* Description: Projektet använder 2st servos som vrids på x & y-axel. Projektet mäter av ljusstyrkan med hjälp av fotoresistorer. Tillsammans hjälper de åt att positionera rätt så att alla fotoresistorer får slutligen samma värde. 
*/

#include <Servo.h>  // Bibliotek Servo
#include <Wire.h>   //

// Servo Variabler
Servo myservoX;   
Servo myservoY;  

// Deifinierar pins för Fotoresistorer och servo
#define L1 A0  // vänster uppe
#define L2 A1  // höger uppe
#define L3 A2  // vänster nere
#define L4 A3  // höger nere
#define SY A4  // servo y-axeln
#define SX A5  // Servo x-axeln

// Variabler
int leftUp;     // vänster uppe
int rightUp;    // höger uppe
int leftDown;   // vänster nere
int rightDown;  // höger nere
int rotationX = 90; // Starposition på servo
int rotationY = 0;  // Starposition på servo
int gransVarde = 50; //gränsvärdet på fotoresistorerna innan servot börjar vända sig
int minX = 0;   //min värde X-axel
int maxX = 180; //max värde X-axel
int minY = 0;   //min värde Y-axel
int maxY = 180; //max värde Y-axel
int step = 1;   //servo gradrotation. Högre värde = snabbare rotation 

void setup() {
  Serial.begin(9600);

 // Sätter pinnarna för fotoresistorer som ingångar
  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  pinMode(L3, INPUT);
  pinMode(L4, INPUT);

 // Kopplar servomotorer till sina respektive pinnar
  myservoY.attach(SY);
  myservoX.attach(SX);
  
  Wire.begin();  // Initierar I2C-kommunikation
}

void loop() {
  // Läs av ljusstyrkevärden från fotoresistorerna
  leftUp = HogerUpp();
  rightUp = VansterUpp();
  leftDown = HogerNer();
  rightDown = VansterNer();

  // Kontrollera om ljusstyrkan är större från övre fotoresistorer
  if (((leftUp + rightUp) - (leftDown + rightDown)) > gransVarde) {
    myservoY.write(rotationY);
    rotationY = rotationY - step;
    if (rotationY < minY) {
      rotationY = minY;
    }
  }
  // Kontrollera om ljusstyrkan är större från nedre fotoresistorer
  else if (((leftDown + rightDown) - (leftUp + rightUp)) > gransVarde) {
    myservoY.write(rotationY);
    rotationY = rotationY + step;
    if (rotationY > maxY) {
      rotationY = maxY;
    }
  }

  // Kontrollera om ljusstyrkan är mer från höger fotoresistorer
  if (((rightUp + rightDown) - (leftUp + leftDown)) < gransVarde) {
    myservoX.write(rotationX);
    rotationX = rotationX + step;
    if (rotationX > maxX) {
      rotationX = maxX;
    }
  } 

  // Kontrollera om ljusstyrkan är mer från vänster fotoresistorer
  else if (((leftUp + leftDown) - (rightUp + rightDown)) < gransVarde) {
    myservoX.write(rotationX);
    rotationX = rotationX - step;
    if (rotationX < minX) {
      rotationX = minX;
    }
  }

  debugger(); // Skriver ut värdena på den seriella monitorn för felsökning
}

// Funktion för att läsa ljusstyrka från höger upp fotoresistor
int HogerUpp() { 
  int x = analogRead(L2);
  return x;
}

// Funktion för att läsa ljusstyrka från vänster upp fotoresistor
int VansterUpp() {
  int x = analogRead(L1);
  return x;
}

// Funktion för att läsa ljusstyrka från höger ner fotoresistor
int HogerNer() {
  int x = analogRead(L4);
  return x;
}

// Funktion för att läsa ljusstyrka från vänster nere fotoresistor
int VansterNer() {
  int x = analogRead(L3);
  return x;
}

// Funktion för att skriva ut debug-information på seriell monitor
void debugger() {
  Serial.print(leftUp); // Skriv ut värdet för vänster uppe
  Serial.print("\t");    // Tabbar för läsbarhet
  Serial.print(leftDown); // Skriv ut värdet för vänster nere
  Serial.print("\t");    
  Serial.print(rightUp); // Skriv ut värdet för höger uppe
  Serial.print("\t");    
  Serial.print(rightDown); // Skriv ut värdet för höger nere
  Serial.print("\t");    
  Serial.print(rightUp + rightDown); // Skriv ut total ljusstyrka för höger
  Serial.print("\t");    
  Serial.print(leftUp + leftDown); // Skriv ut total ljusstyrka för vänster
  Serial.print("\t");    
  Serial.print(rotationX); // Skriv ut aktuell rotation för X-axeln
  Serial.print("\t");    
  Serial.println(rotationY); // Skriv ut aktuell rotation för Y-axeln
}