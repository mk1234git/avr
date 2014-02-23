// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "rf_temp.h"

#define LED_PIN   3
#define RF_TX_PIN 4

void setup()
{
  #ifdef USE_SERIAL
    Serial.begin(9600);	  // Debugging only
    Serial.println("setup");
  #endif
  
    setup_rftemp();
      
    // Initialise the IO and ISR
    pinMode(LED_PIN, OUTPUT);
    int i;
    for(i = 0; i < 5; i++)
    {
        digitalWrite(LED_PIN, false);
        delay(200);
        digitalWrite(LED_PIN, true);
        delay(200);
    }
    vw_set_tx_pin(RF_TX_PIN);
//    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(1000);	 // Bits per sec
    
}

void loop()
{
    uint8_t msg[10];
    
    digitalWrite(LED_PIN, true); // Flash a light to show transmitting
    
    loop_rftemp(msg, sizeof(msg));

    vw_send((uint8_t *)msg, strlen((char*)msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(LED_PIN, false);
    delay(10000);
}
