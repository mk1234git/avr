/*
  Ampel
 
 
 The circuit:
 * green, orange and read LED attached
 * pushbutton attached
 * 10K resistor attached to pin 2 from ground
 
 */

#define USE_SERIAL
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




int secs;
int count;

#include <avr/sleep.h>

void power_save()
{
     #ifdef USE_SERIAL
     Serial.println("timeout -> powersave");
     #endif
     delay(100);
     
     a1.disable();
     a2.disable();
     set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
     sleep_enable();          // enables the sleep bit in the mcucr register
     sleep_mode();            // here the device is actually put to sleep!!
                           // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
     sleep_disable();         // first thing after waking from sleep:
     count = 0;      
     secs = 0;
}

void check_power_save()
{
  count++;
  if(count == 100) //one second
  {
    count = 0;
    secs++;
    
    #ifdef USE_SERIAL
    Serial.print("secs: ");
    Serial.println(secs);
    #endif
    
    if(secs == 120)
    { // powersave
      power_save();
    }
  }
}

void low_battery()
{
  int i;
  for(i = 0; i < 25; i++)
  {
    a1.writeLeds(0xF);
    delay(250);
    a1.writeLeds(0x0);
    delay(250);
  
    #ifdef USE_SERIAL
    Serial.println("low battery");
    #endif
  }
  power_save();
}

void setup() 
{
  secs = 0;
  count = 0;
  
  #ifdef USE_SERIAL
  Serial.begin(9600);
  #endif
  
  delay(100);
  if(readVcc() < 3000L)
  {  
    low_battery();
  }  
}

void loop()
{
  if(a1.readButton() || a2.readButton())
  {
    count = 0;
    secs = 0;
    
    #ifdef USE_SERIAL
    Serial.println(a1.getState());
    #endif
  }
  
  delay(10);
  check_power_save();
  
  #if 0
  #ifdef USE_SERIAL
  if(count == 0)
  {
    Serial.print("VCC: ");
    Serial.print(readVcc());
    Serial.print("\n");
  }
  #endif
  #endif
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

