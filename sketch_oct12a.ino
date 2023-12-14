/*
 File:FireFighter
 Author: Philip Röden
 Date:14-12-2023
 Description: Ett program som känner av om det brinner och om det gör det så starta den en vatten pump som sitter på en step motor som kommer skjuta vatten runt sig för släcka elden
 */

#include <Stepper.h>
int RELAY_PIN = 3;
int FLAMER_PIN = 2;

int motorUppNedPin1 = 8;
int motorUppNedPin2 = 9;
int motorUppNedPin3 = 10;
int motorUppNedPin4 = 11;
int motorSpeed = 10; //variable to set stepper speed
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
void setup() {
  pinMode(2, INPUT); //initialize Flame sensor as an input on pin 2.
  pinMode(LED_BUILTIN, OUTPUT);// initialize digital pin LED_BUILTIN as an output.
  pinMode(RELAY_PIN, OUTPUT);//initialize Relay as an output on pin 3.

  //declare the motor pins as outputs
  pinMode(motorUppNedPin1, OUTPUT);
  pinMode(motorUppNedPin2, OUTPUT);
  pinMode(motorUppNedPin3, OUTPUT);
  pinMode(motorUppNedPin4, OUTPUT);
  Serial.begin(9600);


}
/* Function to set the output for the up and down motor based on the given 'out' value*/
void setOutputUppNed(int out)
{
  // Set the output for motor pins based on the bit values of the lookup table
  digitalWrite(motorUppNedPin1, bitRead(lookup[out], 0));
  digitalWrite(motorUppNedPin2, bitRead(lookup[out], 1));
  digitalWrite(motorUppNedPin3, bitRead(lookup[out], 2));
  digitalWrite(motorUppNedPin4, bitRead(lookup[out], 3));
}

/* Function to move the motor upwards*/
void uppat() {
  for (int i = 7; i >= 0; i--)// Iterate through the lookup table in reverse order to move the motor up
  {
    setOutputUppNed(i);
    delayMicroseconds(motorSpeed);
    Serial.print("Ned ");
  }
}

/* Function to move the motor downwards*/
void nerat() {
  for (int i = 0; i <= 7; i++)  // Iterate through the lookup table to move the motor down
  {
    setOutputUppNed(i);    // Set the motor output for the current step
    delayMicroseconds(motorSpeed);// Introduce a delay to control the motor speed
    Serial.print("Ned ");
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(FLAMER_PIN) == 1 ){//om flamer sensorn känner av att det brinner så startar den brandsläckar
    digitalWrite(LED_BUILTIN, HIGH); // Led ON
    Serial.println("** Warning!!!!   Fire detected!!! **");
    digitalWrite(RELAY_PIN, HIGH); // startar pumpen och kör den
    for (int g = 0; g < 512; g++) {//sätter g som en variabel med värde g och säger den ska köra loopen så länge g<512 och varje gång loopen kör så inkremitera g, loopen kommer då köra ett varv
      uppat();
      if (digitalRead(FLAMER_PIN) == 0){//bryter loopen om värdet på flamer sensorn är 0
        break;
      }
    }
    for (int g = 0; g < 512; g++) {//gör samma som förra for loopen men kommer snurra ett varv till andra varvet
      nerat();
      if ( digitalRead(FLAMER_PIN) == 0){
        break;
      }
    }
    digitalWrite(RELAY_PIN, LOW);  // stänger av pumpen
    delay(0);
  }
  else{// om flame sensorn inte känner av att det brinner
    digitalWrite(LED_BUILTIN, LOW); // Led OFF
    Serial.println("No Fire detected");
  }
  delay(100);
}
