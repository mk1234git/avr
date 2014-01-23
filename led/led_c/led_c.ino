/*
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. 
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

const int greenPin = 8;
const int orangePin = 9;
const int redPin = 10;



void setup() {
  // initialize the LED pin as an output:
  pinMode(greenPin, OUTPUT);      
  pinMode(orangePin, OUTPUT);      
  pinMode(redPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);     
}

enum state {GREEN, ORANGE, RED};
enum state state = GREEN;

void button_event()
{
   if(state == GREEN)
   {
     state = ORANGE;
   }
   else if(state == ORANGE)
   {
     state = RED;
   }
   else if(state == RED)
   {
     state = GREEN;
   }

   //state output decoding   
   if(state == GREEN)
   {
    digitalWrite(greenPin, HIGH);
    digitalWrite(orangePin, LOW);  
    digitalWrite(redPin, LOW);  
   }
   elseif(state == ORANGE)
   {
    digitalWrite(greenPin, LOW);
    digitalWrite(orangePin, HIGH);  
    digitalWrite(redPin, LOW);  
   }
   elseif(state == RED)
   {
    digitalWrite(greenPin, LOW);
    digitalWrite(orangePin, LOW);  
    digitalWrite(redPin, HIGH);  

   }
   
}

void loop(){
    // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        //button press event
        button_event();
      }
    }
  }
  
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
  
#if 0  
  
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(greenPin, HIGH);
    digitalWrite(orangePin, HIGH);  
    digitalWrite(redPin, LOW);  
  } 
  else {
    // turn LED off:
    digitalWrite(greenPin, LOW);
    digitalWrite(orangePin, LOW);  
    digitalWrite(redPin, HIGH);  
  }
 #endif
}
