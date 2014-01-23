// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.setTimeout(10); //ms
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
} 

int posMin = 300; 
int posMax = 2800;
int posStep = 10;
int delayMsec = 50;

void loop()
{
  pos = Serial.parseInt();
  if(pos != 0)
      myservo.write(pos);
  
}

void sweep() 
{ 
  for(pos = posMin; pos < posMax; pos += posStep)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
//    myservo.write(pos);    // tell servo to go to position in variable 'pos' 
    myservo.writeMicroseconds(pos);
    if(pos % 100 == 0)
      Serial.println(pos);
    delay(delayMsec);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = posMax; pos > posMin; pos -= posStep)     // goes from 180 degrees to 0 degrees 
  {                                
     myservo.writeMicroseconds(pos);
//    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    if(pos % 100 == 0)
        Serial.println(pos);
    delay(delayMsec);                       // waits 15ms for the servo to reach the position 
  } 
} 
