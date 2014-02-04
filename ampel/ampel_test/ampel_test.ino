/*
  Ampel
 
 
 The circuit:
 * green, orange and read LED attached
 * pushbutton attached
 * 10K resistor attached to pin 2 from ground
 
 */

//#define USE_SERIAL
#include "ampel.h"

#define BUTTON_1  10
#define GREEN_1   9
#define ORANGE_1  8
#define RED_1     7

#define BUTTON_2  0
#define GREEN_2   1
#define ORANGE_2  2
#define RED_2     3

Ampel a1(BUTTON_1, GREEN_1, ORANGE_1, RED_1);
Ampel a2(BUTTON_2, GREEN_2, ORANGE_2, RED_2);


void setup() 
{
  #ifdef USE_SERIAL
  Serial.begin(9600);
  #endif
}

void loop()
{
  if(a1.readButton())
  {
    #ifdef USE_SERIAL
    Serial.println(a1.getState());
    #endif
  }
  a2.readButton();
  
  delay(10);  
}

