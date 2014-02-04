/*
  Ampel.cpp - Library for ampel control
  Martin Kohlmann, 2014-02-02
  Released into the public domain.
*/

#include "Arduino.h"
#include "ampel.h"


// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long debounceDelay = 20;    // the debounce time; increase if the output flickers


Ampel::Ampel(int _buttonPin, int _greenPin, int _orangePin, int _redPin)
{
  buttonPin = _buttonPin;
  greenPin  = _greenPin;
  orangePin = _orangePin;
  redPin    = _redPin;
  
  // initialize the LED pins as an output:
  pinMode(greenPin, OUTPUT);      
  pinMode(orangePin, OUTPUT);      
  pinMode(redPin, OUTPUT);      
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);

  lastButtonState = LOW;   // the previous reading from the input pin
  lastDebounceTime = 0;
 
  state = GREEN;
  previous_state = state;
  mode = ZUG;  
  
  timeout = 0;
  writeLeds();
}

void Ampel::setNextTimeoutState()
{
  if(millis() >= timeout)
  {
    if(mode == AUTO)
    {
       if(state == ORANGE)
       {
         state = RED;
       }
       else if(state == ORANGE_RED)
       {
         state = GREEN;
       }
       writeLeds();    
    }

  }
}

void Ampel::setNextState()
{
  e_state next_state = state;
  
  if(mode == AUTO)
  {
     if(state == GREEN) {
       next_state = ORANGE;
     }
     else if(state == ORANGE) {
       next_state = RED;
     }
     else if(state == RED) {
       next_state = ORANGE_RED;
     }
     else if(state == ORANGE_RED)  {
       next_state = GREEN;
     }
  }
  else if(mode == ZUG)
  {
     if(state == GREEN) {
       next_state = ORANGE_GREEN;
     }
     else if(state == ORANGE_GREEN) {
       if(previous_state == RED)
           next_state = GREEN;
       else
           next_state = RED;
     }
     else if(state == RED) {
       next_state = GREEN;
     }
  }
  if(state != next_state)
  {
    previous_state = state;
    state = next_state;
    
    writeLeds();   
  }
}


void Ampel::writeLeds()
{
   //state output decoding   
   if(state == GREEN)
   {
    digitalWrite(greenPin, LOW);
    digitalWrite(orangePin, HIGH);  
    digitalWrite(redPin, HIGH);  
   }
   else if(state == ORANGE)
   {
    digitalWrite(greenPin, HIGH);
    digitalWrite(orangePin, LOW);  
    digitalWrite(redPin, HIGH);  
   }
   else if(state == RED)
   {
    digitalWrite(greenPin, HIGH);
    digitalWrite(orangePin, HIGH);  
    digitalWrite(redPin, LOW);  
   }
   else if(state == ORANGE_RED)
   {
    digitalWrite(greenPin, HIGH);
    digitalWrite(orangePin, LOW);  
    digitalWrite(redPin, LOW);  
   }
   else if(state == ORANGE_GREEN)
   {
    digitalWrite(greenPin, LOW);
    digitalWrite(orangePin, LOW);  
    digitalWrite(redPin, HIGH);  
   }
}

void Ampel::setState(e_state newState)
{
    previous_state = state;
    state = newState;
    writeLeds();
}

char Ampel::getState()
{
    switch(state)
    {
        case GREEN:  return 'G';
        case ORANGE: return 'O';
        case RED:    return 'R';
        case ORANGE_RED:   return 'r';
        case ORANGE_GREEN: return 'g';
        default: return ' ';
    }
}

bool Ampel::readButton()
{
  bool buttonPressEvent = false;
  int reading = !digitalRead(buttonPin);
    
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
        buttonPressEvent = true;
        setNextState();
        timeout = millis() + 500;
      }
    }
  }
  
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

  return buttonPressEvent;
}
