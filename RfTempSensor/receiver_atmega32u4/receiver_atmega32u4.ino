// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "rf_temp.h"

#define LED_PIN   12
#define RF_RX_PIN 3


void setup()
{
    Serial.begin(9600);	// Debugging only
    Serial.println("setup");

    setup_rftemp();

    // Initialise the IO and ISR
    vw_set_rx_pin(RF_RX_PIN);
//    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(1000);	 // Bits per sec

    vw_rx_start();       // Start the receiver PLL running
}

void readPrintTemp(uint8_t *p)
{
        uint8_t addr = p[0];
        int16_t tRaw = (p[1] << 8) | p[2];
        float tC = sensors.rawToCelsius(tRaw);
        
        Serial.print(addr, HEX);
        Serial.print(": ");
        Serial.print(tRaw, DEC);
        Serial.print(" raw, ");
        Serial.print(tC, DEC);
        Serial.println(" C.");  
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

//    loop_rftemp();
//    delay(1000);
//    Serial.println("loop");

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
	int i;

        digitalWrite(LED_PIN, true); // Flash a light to show received good message
	// Message with a good checksum received, dump it.
	Serial.print("Got: ");
	
	for (i = 0; i < buflen; i++)
	{
	    Serial.print(buf[i], HEX);
	    Serial.print(" ");
	}
	Serial.println("");

        Serial.print("count: ");
        Serial.println(buf[1], DEC);

        readPrintTemp(buf + 2);
        readPrintTemp(buf + 5);
        
        digitalWrite(LED_PIN, false);
    }
}
