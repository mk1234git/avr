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

 
 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin

// variables will change:
int reading = 0;         // variable for reading the pushbutton status

const int greenPin = 8;
const int orangePin = 9;
const int redPin = 10;


int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 20;    // the debounce time; increase if the output flickers


enum state {GREEN, ORANGE_GREEN, ORANGE, ORANGE_RED, RED};
enum state state;
enum staet previous_state;

enum mode {AUTO, ZUG};
enum mode mode;



void setup() {
  // initialize the LED pin as an output:
  pinMode(greenPin, OUTPUT);      
  pinMode(orangePin, OUTPUT);      
  pinMode(redPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  state = GREEN;
  previous_state = state;
  mode = ZUG;  
  set_leds();
}


void button_event()
{
  state next_state = state;
  
  if(mode == AUTO)
  {
     if(state == GREEN)
     {
       next_state = ORANGE;
     }
     else if(state == ORANGE)
     {
       next_state = RED;
     }
     else if(state == RED)
     {
       next_state = ORANGE_RED;
     }
     else if(state == ORANGE_RED)
     {
       next_state = GREEN;
     }
  }
  else if(mode == ZUG)
  {
     if(state == GREEN)
     {
       next_state = ORANGE_GREEN;
     }
     else if(state == ORANGE_GREEN)
     {
       if(previous_state == RED)
           next_state = GREEN;
       else
           next_state = RED;
     }
     else if(state == RED)
     {
       next_state = GREEN;
     }
  }
  if(state != next_state)
  {
    previous_state = state;
    state = next_state;
    
    set_leds();   
  }
}


long timeout = 0;


void state_tick()
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
       set_leds();    
    }

  }

}

void set_leds()
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

void loop(){
  
    int reading = digitalRead(buttonPin);
    
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
        timeout = millis() + 500;
      }
    }
  }
  
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

  state_tick();
  delay(10);  
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
